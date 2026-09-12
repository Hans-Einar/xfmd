#include "LocalFileStore.h"
#include "InputPolicy.h"
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>
#include <vector>
namespace xfmd {
namespace fs = std::filesystem;
namespace {
struct Descriptor {
  int fd;
  explicit Descriptor(int fd) : fd(fd) {}
  ~Descriptor() { if (fd >= 0) ::close(fd); }
  Descriptor(const Descriptor&) = delete;
};
[[noreturn]] void io(const std::string& operation) {
  throw Error(ErrorCode::Io, operation + ": " + std::strerror(errno));
}
FileIdentity identity(const struct stat& st, const std::string& bytes) {
  std::uint64_t hash = 14695981039346656037ULL;
  for (unsigned char c : bytes) { hash ^= c; hash *= 1099511628211ULL; }
  return {std::uint64_t(st.st_dev), std::uint64_t(st.st_ino), hash,
          st.st_mtim.tv_sec * 1000000000LL + st.st_mtim.tv_nsec,
          st.st_ctim.tv_sec * 1000000000LL + st.st_ctim.tv_nsec, bytes.size()};
}
std::string resolved(const std::string& name) {
  std::error_code error;
  auto result = fs::weakly_canonical(fs::absolute(name), error);
  if (error) throw Error(ErrorCode::Io, "Cannot resolve file: " + error.message());
  return result.string();
}
void copyAttributes(int from, int to) {
  auto size = flistxattr(from, nullptr, 0);
  if (size < 0 && (errno == ENOTSUP || errno == EOPNOTSUPP)) return;
  if (size < 0) io("Read file attributes");
  std::vector<char> names(static_cast<std::size_t>(size));
  if (size && flistxattr(from, names.data(), names.size()) != size) io("Read attribute names");
  for (std::size_t i = 0; i < names.size();) {
    const char* name = names.data() + i;
    auto length = fgetxattr(from, name, nullptr, 0);
    if (length < 0) io("Read attribute");
    std::vector<char> value(static_cast<std::size_t>(length));
    if (fgetxattr(from, name, value.data(), value.size()) != length) io("Read attribute value");
    if (fsetxattr(to, name, value.data(), value.size(), 0) < 0) io("Preserve file attributes");
    i += std::strlen(name) + 1;
  }
}
}
bool FileIdentity::operator==(const FileIdentity& b) const {
  return device == b.device && inode == b.inode && hash == b.hash && modified == b.modified &&
         changed == b.changed && size == b.size;
}
LoadedDocument LocalFileStore::read(const std::string& name) const {
  InputPolicy::validate({}, name);
  std::string path = resolved(name);
  Descriptor file(::open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NONBLOCK));
  if (file.fd < 0) io("Open " + path);
  struct stat before{}, after{};
  if (fstat(file.fd, &before) < 0) io("Inspect file");
  if (!S_ISREG(before.st_mode)) throw Error(ErrorCode::Unsupported, "Only regular files can be opened.");
  if (before.st_size > static_cast<off_t>(maxDocumentBytes)) throw Error(ErrorCode::TooLarge, "File exceeds 8 MiB.");
  std::string bytes;
  char buffer[16384];
  for (;;) {
    auto count = ::read(file.fd, buffer, sizeof(buffer));
    if (count < 0) { if (errno == EINTR) continue; io("Read file"); }
    if (!count) break;
    bytes.append(buffer, static_cast<std::size_t>(count));
    if (bytes.size() > maxDocumentBytes) throw Error(ErrorCode::TooLarge, "File exceeds 8 MiB.");
  }
  if (fstat(file.fd, &after) < 0) io("Inspect loaded file");
  if (!(identity(before, bytes) == identity(after, bytes)))
    throw Error(ErrorCode::Conflict, "File changed while it was being read. Try again.");
  InputPolicy::validate(bytes, name);
  return {path, bytes, identity(after, bytes), InputPolicy::plainText(name)};
}
SavedDocument LocalFileStore::writeAtomic(const SourceSnapshot& source, const std::string& name,
                                         const std::optional<FileIdentity>& expected) const {
  InputPolicy::validate(source.text, name);
  const auto path = resolved(name);
  struct stat original{};
  bool exists = ::stat(path.c_str(), &original) == 0;
  if (!exists && errno != ENOENT) io("Inspect save target");
  if (exists && !expected) throw Error(ErrorCode::Conflict, "Target already exists; confirm overwrite first.");
  if (expected && (!exists || !(read(path).identity == *expected)))
    throw Error(ErrorCode::Conflict, "File changed externally. Reload or Save As another name.");
  if (exists && original.st_nlink > 1) throw Error(ErrorCode::Unsupported, "File has hard links. Use Save As another name.");
  Descriptor old(exists ? ::open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW) : -1);
  if (exists && old.fd < 0) io("Open metadata source");
  std::string temporary = path + ".xfmd-XXXXXX";
  std::vector<char> tempName(temporary.begin(), temporary.end());
  tempName.push_back(0);
  Descriptor temp(::mkstemp(tempName.data()));
  if (temp.fd < 0) io("Create temporary file");
  struct Cleanup { const char* name; ~Cleanup() { ::unlink(name); } } cleanup{tempName.data()};
  std::size_t written = 0;
  while (written < source.text.size()) {
    auto count = ::write(temp.fd, source.text.data() + written, source.text.size() - written);
    if (count < 0) { if (errno == EINTR) continue; io("Write file"); }
    if (!count) { errno = EIO; io("Write file"); }
    written += static_cast<std::size_t>(count);
  }
  if (exists) {
    if (fchown(temp.fd, original.st_uid, original.st_gid) < 0) io("Preserve file owner");
    if (fchmod(temp.fd, original.st_mode & 0777) < 0) io("Preserve file permissions");
    copyAttributes(old.fd, temp.fd);
  }
  if (::fsync(temp.fd) < 0) io("Flush file");
  if (checkpoint) checkpoint("before-rename");
  if (expected && !(read(path).identity == *expected)) throw Error(ErrorCode::Conflict, "File changed before replacement.");
  if (!expected) {
    // link publishes a new file without overwriting a target created by another writer.
    if (::link(tempName.data(), path.c_str()) < 0) io("Publish new file");
    ::unlink(tempName.data());
  } else if (::rename(tempName.data(), path.c_str()) < 0) io("Replace file");
  struct stat saved{};
  if (::fstat(temp.fd, &saved) < 0) io("Inspect saved file");
  Descriptor directory(::open(fs::path(path).parent_path().c_str(), O_RDONLY | O_DIRECTORY | O_CLOEXEC));
  bool durable = directory.fd >= 0 && ::fsync(directory.fd) == 0;
  return {identity(saved, source.text), durable, path};
}
}
