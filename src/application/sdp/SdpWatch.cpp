#include "SdpWatch.h"
#include "SdpProtocol.h"
#include <filesystem>
#include <map>
#include <stdexcept>
namespace xfmd {
namespace {
namespace fs = std::filesystem;
using Stamp = std::pair<fs::file_time_type, std::uintmax_t>;
using Snapshot = std::map<std::string, Stamp>;
Snapshot scan(const std::string& root, const std::atomic<bool>& stopped) {
  Snapshot result;
  const auto area = sdpArea(root);
  if (area.empty())
    return result;
  result.emplace(area, Stamp{fs::last_write_time(area), 0});
  std::size_t entries = 0;
  for (fs::recursive_directory_iterator it(area), end; it != end && !stopped; ++it) {
    if (++entries > 20000 || it.depth() >= 32)
      throw std::runtime_error("SDP watch size/depth limit reached");
    auto type = it->symlink_status().type();
    // Never follow or stat a symlink's destination. Parent mtime records its replacement.
    if (type == fs::file_type::symlink) {
      it.disable_recursion_pending();
      result.emplace(it->path().string(), Stamp{});
    } else if (type == fs::file_type::regular || type == fs::file_type::directory) {
      result.emplace(
          it->path().string(),
          Stamp{it->last_write_time(), type == fs::file_type::regular ? it->file_size() : 0});
    }
  }
  return result;
}
} // namespace
SdpWatch::~SdpWatch() { stop(); }
void SdpWatch::stop() {
  {
    std::lock_guard<std::mutex> lock(mutex);
    stopped = true;
  }
  wake.notify_all();
  if (worker.joinable())
    worker.join();
}
void SdpWatch::start(const std::string& root) {
  stop();
  {
    std::lock_guard<std::mutex> lock(mutex);
    changed = false;
    error.clear();
  }
  stopped = false;
  worker = std::thread([this, root] {
    Snapshot previous;
    bool initial = true, pending = false;
    auto since = std::chrono::steady_clock::now();
    unsigned failures = 0;
    while (!stopped) {
      try {
        auto current = scan(root, stopped);
        if (stopped)
          break;
        failures = 0;
        if (initial) {
          std::lock_guard<std::mutex> lock(mutex);
          changed = true; // Reconcile discovery with the initial watch baseline.
        }
        const bool different = initial || current != previous;
        const auto now = std::chrono::steady_clock::now();
        if (different && !initial && !pending) {
          pending = true;
          since = now;
        }
        if (pending && (!different || now - since >= std::chrono::seconds(2))) {
          std::lock_guard<std::mutex> lock(mutex);
          changed = true;
          pending = false;
        }
        initial = false;
        previous = std::move(current);
      } catch (const std::exception& e) {
        // A concurrent rename can invalidate one traversal; retry transient failures.
        if (++failures >= 3) {
          std::lock_guard<std::mutex> lock(mutex);
          error = std::string(e.what()) + "; use Refresh to retry";
          break;
        }
      }
      std::unique_lock<std::mutex> lock(mutex);
      wake.wait_for(lock, std::chrono::milliseconds(400), [this] { return stopped.load(); });
    }
  });
}
bool SdpWatch::take(std::string& diagnostic) {
  std::lock_guard<std::mutex> lock(mutex);
  diagnostic = std::move(error);
  error.clear();
  const bool result = changed;
  changed = false;
  return result;
}
} // namespace xfmd
