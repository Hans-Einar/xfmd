#pragma once
#include <filesystem>
namespace xfmd {
// Allocate lazily for one window; the caller owns and removes the returned workspace.
std::filesystem::path sdpRuntimeWorkspace();
} // namespace xfmd
