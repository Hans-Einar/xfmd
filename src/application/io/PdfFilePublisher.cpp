#include "PdfFilePublisher.h"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <unistd.h>
#include <vector>
namespace xfmd {
namespace {
[[noreturn]] void fail(const char* operation) {
  throw Error(ErrorCode::Io, std::string(operation) + ": " + std::strerror(errno));
}
bool same(const struct stat& a, const struct stat& b) {
  return a.st_dev == b.st_dev && a.st_ino == b.st_ino && a.st_size == b.st_size &&
         a.st_mtim.tv_sec == b.st_mtim.tv_sec && a.st_mtim.tv_nsec == b.st_mtim.tv_nsec &&
         a.st_ctim.tv_sec == b.st_ctim.tv_sec && a.st_ctim.tv_nsec == b.st_ctim.tv_nsec;
}
} // namespace
PdfTarget PdfTarget::inspect(const std::string& name) {
  auto absolute = std::filesystem::absolute(name);
  PdfTarget value;
  value.path = (std::filesystem::canonical(absolute.parent_path()) / absolute.filename()).string();
  if (::lstat(value.path.c_str(), &value.identity) == 0) {
    value.exists = true;
    if (!S_ISREG(value.identity.st_mode) || value.identity.st_nlink != 1)
      throw Error(ErrorCode::Unsupported,
                  "Choose a regular PDF target without symbolic or hard links.");
  } else if (errno != ENOENT)
    fail("Inspect PDF target");
  return value;
}
PdfFilePublisher::PdfFilePublisher(PdfTarget value) : target(std::move(value)) {
  std::string pattern = target.path + ".xfmd-pdf-XXXXXX";
  std::vector<char> name(pattern.begin(), pattern.end());
  name.push_back(0);
  descriptor = ::mkostemp(name.data(), O_CLOEXEC);
  if (descriptor < 0)
    fail("Create PDF temporary file");
  temporary = name.data();
}
PdfFilePublisher::~PdfFilePublisher() {
  if (descriptor >= 0)
    ::close(descriptor);
  if (!temporary.empty())
    ::unlink(temporary.c_str());
}
void PdfFilePublisher::commit(ExportControl& control) {
  control.checkpoint();
  if (target.exists && ::fchmod(descriptor, target.identity.st_mode & 0777) < 0)
    fail("Preserve PDF permissions");
  if (::fsync(descriptor) < 0)
    fail("Flush PDF");
  std::lock_guard<std::mutex> lock(control.publication);
  control.checkpoint();
  auto current = PdfTarget::inspect(target.path);
  if (current.exists != target.exists ||
      (target.exists && !same(current.identity, target.identity)))
    throw Error(ErrorCode::Conflict, "PDF target changed during export; choose a new target.");
  if (target.exists) {
    if (::rename(temporary.c_str(), target.path.c_str()) < 0)
      fail("Replace PDF");
  } else {
    if (::link(temporary.c_str(), target.path.c_str()) < 0)
      fail("Publish PDF");
    ::unlink(temporary.c_str());
  }
  control.published = true;
  // After this point cancellation cannot undo publication. Directory fsync is best effort.
  int directory = ::open(std::filesystem::path(target.path).parent_path().c_str(),
                         O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  if (directory >= 0) {
    ::fsync(directory);
    ::close(directory);
  }
}
} // namespace xfmd
