#ifndef AGARPLUSPLUS_GRIDSTYPES_H
#define AGARPLUSPLUS_GRIDSTYPES_H

#include <functional>
#include <memory>
#include <set>

#include "game/GameObject.h"
#include "grid/SpatialGrid.h"

class GameObject;
class SpatialGrid;

using CollisionHandler =  std::function<void(GameObject *, GameObject *)>;

using Comparator = std::function<bool(std::unique_ptr<SpatialGrid> const &, std::unique_ptr<SpatialGrid> const &)>;

using Grids = std::set<std::unique_ptr<SpatialGrid>, Comparator>;

#endif
