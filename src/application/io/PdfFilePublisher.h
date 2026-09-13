#pragma once
#include "application/export/ExportCoordinator.h"
#include <sys/stat.h>
namespace xfmd {
struct PdfTarget {
  std::string path;
  bool exists = false;
  struct stat identity{};
  static PdfTarget inspect(const std::string&);
};
class PdfFilePublisher {
  PdfTarget target;
  std::string temporary;
  int descriptor = -1;

public:
  explicit PdfFilePublisher(PdfTarget);
  ~PdfFilePublisher();
  PdfFilePublisher(const PdfFilePublisher&) = delete;
  PdfFilePublisher& operator=(const PdfFilePublisher&) = delete;
  int fd() const { return descriptor; }
  void commit(ExportControl&);
};
} // namespace xfmd
