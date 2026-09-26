#pragma once
#include <fx.h>
namespace xfmd {
enum class PathMenuAction { None, SetWorkPath, AddFavorite, RemoveFavorite };
// Returns after the popup closes. Callers retain paths, never item pointers.
PathMenuAction showPathContextMenu(FX::FXWindow*, const FX::FXEvent&, PathMenuAction,
                                   bool offerWorkPath = false);
} // namespace xfmd
