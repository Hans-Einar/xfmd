#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
// ABI v1: bounded, little-endian value records. Data ownership stays in Rust.
typedef struct XfmdDiagramResult {
  uint32_t abi_version, struct_size, status;
  const uint8_t* data;
  uint64_t size;
  void* owner;
} XfmdDiagramResult;
XfmdDiagramResult xfmd_mermaid_parse_v1(uint32_t abi, const uint8_t* data, uint64_t size);
XfmdDiagramResult xfmd_diagram_layout_v1(uint32_t abi, const uint8_t* data, uint64_t size);
void xfmd_mermaid_parse_free_v1(XfmdDiagramResult*);
void xfmd_diagram_layout_free_v1(XfmdDiagramResult*);
#ifdef __cplusplus
}
#endif
