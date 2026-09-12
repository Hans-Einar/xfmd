#pragma once
#include "HistoryStore.h"
#include "application/document/DocumentCoordinator.h"
#include "application/scroll/ScrollCoordinator.h"
namespace xfmd {
class NavigationCoordinator {
  DocumentCoordinator& documents;
  DocumentSession& session;
  ScrollCoordinator& scrolling;
  struct Request {
    SourceAnchor previous;
    std::optional<std::size_t> target;
  };
  std::optional<Request> pending;

public:
  HistoryStore history;
  std::function<void(const std::string&)> error;
  NavigationCoordinator(DocumentCoordinator& d, DocumentSession& s, ScrollCoordinator& scroll)
      : documents(d), session(s), scrolling(scroll) {}
  bool openTarget(const std::string&, std::optional<std::size_t> = {});
  bool followLink(const std::string&);
  bool goBack();
  bool goForward();
  void commitVisit();
  void documentSaved();
};
} // namespace xfmd
