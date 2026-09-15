#include "DirectoryScanner.h"
#include "WorkPathHistory.h"
namespace xfmd {
namespace fs = std::filesystem;
DirectoryScanner::~DirectoryScanner() { stop(); }
void DirectoryScanner::stop() {
  {
    // Coordinate the predicate transition with wait() to avoid a lost wakeup.
    std::lock_guard<std::mutex> lock(mutex);
    cancelled = true;
  }
  ready.notify_all();
  if (worker.joinable())
    worker.join();
}
void DirectoryScanner::start(const fs::path& path, FileNameFilter value) {
  stop();
  root = path;
  filter = std::move(value);
  {
    std::lock_guard<std::mutex> lock(mutex);
    pending.clear();
    jobs.clear();
    errors = 0;
    working = false;
    jobs.push_back(root);
  }
  cancelled = false;
  worker = std::thread([this] { run(); });
}
void DirectoryScanner::request(const fs::path& path) {
  if (!WorkPathHistory::contains(root, path))
    return;
  {
    std::lock_guard<std::mutex> lock(mutex);
    jobs.push_back(path);
  }
  ready.notify_all();
}
ScanBatch DirectoryScanner::take() {
  ScanBatch batch;
  {
    std::lock_guard<std::mutex> lock(mutex);
    for (unsigned n = 0; n < 512 && !pending.empty(); ++n) {
      batch.entries.push_back(std::move(pending.front()));
      pending.pop_front();
    }
    batch.busy = working || !jobs.empty() || !pending.empty();
    batch.errors = errors;
  }
  ready.notify_all();
  return batch;
}
void DirectoryScanner::publish(TreeEntry entry) {
  std::unique_lock<std::mutex> lock(mutex);
  ready.wait(lock, [this] { return cancelled || pending.size() < 4096; });
  if (!cancelled)
    pending.push_back(std::move(entry));
}
void DirectoryScanner::run() {
  while (!cancelled) {
    fs::path path;
    {
      std::unique_lock<std::mutex> lock(mutex);
      ready.wait(lock, [this] { return cancelled || !jobs.empty(); });
      if (cancelled)
        return;
      path = std::move(jobs.front());
      jobs.pop_front();
      working = true;
    }
    try {
      scan(path);
    } catch (const fs::filesystem_error&) {
      std::lock_guard<std::mutex> lock(mutex);
      ++errors;
    }
    {
      std::lock_guard<std::mutex> lock(mutex);
      working = false;
    }
  }
}
void DirectoryScanner::scan(const fs::path& path) {
  std::vector<fs::path> directories{path};
  auto failed = [this] {
    std::lock_guard<std::mutex> lock(mutex);
    ++errors;
  };
  while (!directories.empty() && !cancelled) {
    auto directory = std::move(directories.back());
    directories.pop_back();
    std::error_code ec;
    auto resolved = fs::canonical(directory, ec);
    if (ec || !WorkPathHistory::contains(root, resolved)) {
      failed();
      continue;
    }
    if (directory != root && fs::is_symlink(directory, ec))
      continue;
    fs::directory_iterator it(directory, ec), end;
    if (ec) {
      failed();
      continue;
    }
    while (it != end && !cancelled) {
      const auto entry = *it;
      auto status = entry.symlink_status(ec);
      if (ec) {
        failed();
        ec.clear();
      } else if (fs::is_directory(status)) {
        if (filter.active())
          directories.push_back(entry.path());
        else
          publish({entry.path(), true});
      } else {
        bool regular = fs::is_regular_file(status);
        if (fs::is_symlink(status)) {
          auto target = fs::canonical(entry.path(), ec);
          regular =
              !ec && WorkPathHistory::contains(root, target) && fs::is_regular_file(target, ec);
        }
        if (regular && filter.matches(entry.path().filename().string()))
          publish({entry.path(), false});
      }
      it.increment(ec);
      if (ec) {
        failed();
        break;
      }
    }
  }
}
} // namespace xfmd
