#pragma once
#include "contracts/diagram/DiagramAbi.h"
#ifdef __cplusplus
extern "C" {
#endif
// Separate BoxUI JSON envelope. Ownership and synchronous callback lifetime match
// DiagramAbi; no C++ exception or Rust panic may cross the boundary.
typedef uint32_t (*XfmdBoxUiCancel)(void*);
typedef uint32_t (*XfmdBoxUiMeasure)(void*, const uint8_t*, uint64_t, double, double*, double*,
                                     double*);
XfmdDiagramResult xfmd_boxui_parse_v1(uint32_t, const uint8_t*, uint64_t);
XfmdDiagramResult xfmd_boxui_prepare_v1(uint32_t, const uint8_t*, uint64_t, void*, XfmdBoxUiMeasure,
                                        XfmdBoxUiCancel);
void xfmd_boxui_result_free_v1(XfmdDiagramResult*);
#ifdef __cplusplus
}
#endif
