#pragma once
#include "contracts/DocumentTypes.h"
#include <vector>
namespace xfmd {
// GdkPixbuf may retain every GIF frame. Bound the aggregate before decoding.
inline void checkGifBudget(const std::vector<unsigned char>& bytes) {
  if (bytes.size() < 13 || std::string(reinterpret_cast<const char*>(bytes.data()), 3) != "GIF")
    return;
  auto word = [&](std::size_t at) {
    return unsigned(bytes.at(at)) | (unsigned(bytes.at(at + 1)) << 8);
  };
  const std::uint64_t pixels = std::uint64_t(word(6)) * word(8);
  std::size_t at = 13, frames = 0;
  if (bytes[10] & 128)
    at += 3u << ((bytes[10] & 7) + 1);
  auto subBlocks = [&] {
    while (at < bytes.size()) {
      auto count = bytes.at(at++);
      if (!count)
        return;
      if (count > bytes.size() - at)
        throw Error(ErrorCode::InvalidInput, "Truncated GIF.");
      at += count;
    }
    throw Error(ErrorCode::InvalidInput, "Truncated GIF.");
  };
  while (at < bytes.size()) {
    auto marker = bytes[at++];
    if (marker == 0x3b)
      return;
    if (marker == 0x21) {
      ++at;
      subBlocks();
    } else if (marker == 0x2c) {
      if (at + 9 > bytes.size())
        throw Error(ErrorCode::InvalidInput, "Truncated GIF.");
      if (++frames > 256 || pixels * frames > 16000000)
        throw Error(ErrorCode::TooLarge, "GIF frame budget exceeded.");
      auto packed = bytes[at + 8];
      at += 9;
      if (packed & 128)
        at += 3u << ((packed & 7) + 1);
      ++at; // LZW code size
      subBlocks();
    } else
      throw Error(ErrorCode::InvalidInput, "Invalid GIF block.");
  }
}
} // namespace xfmd
