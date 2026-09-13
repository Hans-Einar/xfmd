#pragma once
#include "FileNameFilter.h"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <mutex>
#include <thread>
#include <vector>
namespace xfmd {
struct TreeEntry {
  std::filesystem::path path;
  bool directory;
};
struct ScanBatch {
  std::vector<TreeEntry> entries;
  bool busy;
  unsigned errors;
};
class DirectoryScanner {
  std::filesystem::path root;
  FileNameFilter filter;
  std::thread worker;
  std::atomic<bool> cancelled{false};
  std::mutex mutex;
  std::condition_variable ready;
  std::deque<std::filesystem::path> jobs;
  std::deque<TreeEntry> pending;
  bool working = false;
  unsigned errors = 0;
  void run();
  void scan(const std::filesystem::path&);
  void publish(TreeEntry);

public:
  ~DirectoryScanner();
  void stop();
  void start(const std::filesystem::path&, FileNameFilter);
  void request(const std::filesystem::path&);
  ScanBatch take();
};
} // namespace xfmd
