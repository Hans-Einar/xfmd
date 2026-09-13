#pragma once
#include <fx.h>
using namespace FX;

// Static illustration, deliberately independent of XFMD's document renderer.
class DocumentFixture : public FXCanvas {
  FXDECLARE(DocumentFixture)
  FXFont *title = nullptr, *heading = nullptr, *body = nullptr, *mono = nullptr;

protected:
  DocumentFixture() = default;

public:
  explicit DocumentFixture(FXComposite*);
  ~DocumentFixture() override;
  void create() override;
  long onPaint(FXObject*, FXSelector, void*);
};
