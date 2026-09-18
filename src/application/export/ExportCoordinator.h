#pragma once
#include "contracts/IRenderer.h"
#include "contracts/boxui/BoxUiFrame.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
namespace xfmd {
struct ExportRequest {
  SourceSnapshot source;
  PaperSpec paper;
  FontSetId fonts = 0;
  std::string target;
  LayoutResult frame;
  BoxUiState boxUi;
};
struct ExportControl {
  std::atomic<bool> cancelled{false};
  std::atomic<unsigned> page{0}, total{0};
  std::mutex publication;
  bool published = false; // guarded by publication
  void checkpoint() const;
};
struct ExportResult {
  DocumentToken token;
  std::string target, error;
  bool success = false, cancelled = false;
  unsigned pages = 0;
};
class ExportCoordinator {
public:
  using Work = std::function<unsigned(const ExportRequest&, ExportControl&)>;

private:
  Work work;
  std::thread worker;
  std::unique_ptr<ExportControl> control;
  std::atomic<bool> done{false};
  ExportResult result;

public:
  explicit ExportCoordinator(Work task) : work(std::move(task)) {}
  ~ExportCoordinator();
  bool start(ExportRequest);
  void cancel();
  bool busy() const { return worker.joinable(); }
  std::pair<unsigned, unsigned> progress() const;
  std::optional<ExportResult> take();
};
} // namespace xfmd
