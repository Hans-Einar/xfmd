#include "TextProjection.h"
#include <algorithm>
namespace xfmd {
TextProjection::TextProjection(const std::string& raw) {
  std::size_t i = raw.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3 : 0;
  auto first = raw.find('\n');
  if (first != std::string::npos && first && raw[first-1] == '\r') newline = "\r\n";
  while (i < raw.size()) {
    offsets.push_back(i);
    if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n') { text += '\n'; i += 2; }
    else { text += raw[i++]; }
  }
  offsets.push_back(raw.size());
}
std::size_t TextProjection::sourceOffset(std::size_t projected) const { return offsets[std::min(projected, text.size())]; }
std::size_t TextProjection::displayOffset(std::size_t source) const {
  return std::min(text.size(), std::size_t(std::lower_bound(offsets.begin(), offsets.end(), source) - offsets.begin()));
}
Edit TextProjection::difference(const std::string& edited) const {
  std::size_t begin = 0, oldEnd = text.size(), newEnd = edited.size();
  while (begin < oldEnd && begin < newEnd && text[begin] == edited[begin]) ++begin;
  // A byte diff may start inside a multi-byte character; expand to a code point boundary.
  while (begin && begin < text.size() && (static_cast<unsigned char>(text[begin]) & 0xc0) == 0x80) --begin;
  while (oldEnd > begin && newEnd > begin && text[oldEnd - 1] == edited[newEnd - 1]) { --oldEnd; --newEnd; }
  while (oldEnd < text.size() && (static_cast<unsigned char>(text[oldEnd]) & 0xc0) == 0x80) { ++oldEnd; ++newEnd; }
  std::string replacement;
  for (std::size_t i = begin; i < newEnd; ++i) {
    if (edited[i] == '\r' && i + 1 < newEnd && edited[i + 1] == '\n') continue;
    if (edited[i] == '\n') replacement += newline;
    else replacement += edited[i];
  }
  return {sourceOffset(begin), sourceOffset(oldEnd), replacement};
}
}
