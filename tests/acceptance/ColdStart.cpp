#include "application/Application.h"
#include <chrono>
#include <iostream>
#include <sys/resource.h>
#include <thread>
int main(int argc, char** argv) {
  if (argc != 2)
    return 2;
  auto start = std::chrono::steady_clock::now();
  xfmd::Application application;
  application.initialize(argc, argv);
  application.documents.error = [](const std::string& error) { throw std::runtime_error(error); };
  if (!application.open(argv[1]))
    return 1;
  while (!application.host->interactive()) {
    application.app.runWhileEvents();
    if (std::chrono::steady_clock::now() - start > std::chrono::seconds(10))
      return 3;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  application.app.runWhileEvents();
  auto ms =
      std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
  struct rusage usage{};
  getrusage(RUSAGE_SELF, &usage);
  std::cout << "first_view_ms=" << ms << " peak_rss_kib=" << usage.ru_maxrss << '\n';
}
