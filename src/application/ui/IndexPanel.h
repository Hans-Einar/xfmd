#pragma once
#include "NavigationTree.h"
#include "application/index/DocumentIndex.h"
#include "application/index/ReferenceWorker.h"
#include <map>
#include <set>
namespace xfmd {
class IndexPanel : public FX::FXVerticalFrame {
  FXDECLARE(IndexPanel)
  DocumentToken token;
  std::string documentPath;
  bool valid = false;
  std::map<std::string, NavigationItem*> files;
  std::set<std::string> loading, requests;
  void request(NavigationItem*);

protected:
  IndexPanel() = default;

public:
  enum { ID_REQUEST = FX::FXVerticalFrame::ID_LAST, ID_LAST };
  NavigationTree *outline = nullptr, *references = nullptr;
  std::function<void(const IndexAction&)> activated;
  std::function<void(const std::string&)> referenceRequested;
  explicit IndexPanel(FX::FXComposite*);
  ~IndexPanel() override;
  long onRequest(FX::FXObject*, FX::FXSelector, void*);
  void invalidate(DocumentToken, const std::string& path);
  void present(const DocumentIndex&, const std::string&);
  void presentReference(const ReferenceResult&);
};
} // namespace xfmd
