#pragma once
#include <cstdint>
namespace xfmd {
// Sanitizer interceptors substantially slow allocations and memory operations
// inside the Rust dependency. Retain a finite diagnostic budget without changing
// the production deadline or disabling any memory checks.
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define XFMD_DIAGRAM_ASAN 1
#endif
#endif
#if defined(__SANITIZE_ADDRESS__) || defined(XFMD_DIAGRAM_ASAN)
inline constexpr std::uint32_t diagramLayoutBudgetMilliseconds = 10000;
#else
inline constexpr std::uint32_t diagramLayoutBudgetMilliseconds = 2000;
#endif
#undef XFMD_DIAGRAM_ASAN
} // namespace xfmd
