#ifndef AGAR_HIERARCHICALGRID_H
#define AGAR_HIERARCHICALGRID_H

#include "game/GameObject.h"
#include "grid/GridsTypes.h"

class HierarchicalGrid {

public:

    static constexpr double CELL_SIZE_SCALE_FACTOR = 2.0;

    static constexpr double CELL_SIZE_SCALE_FACTOR_INVERSE = 1 / CELL_SIZE_SCALE_FACTOR;

    static constexpr double CELL_SIZE_TO_OBJECT_AABB_EXTENT_RATIO = 4.0;

    static constexpr int INITIAL_CELL_CAPACITY = static_cast<int>(2 * (CELL_SIZE_SCALE_FACTOR * CELL_SIZE_SCALE_FACTOR));

    static constexpr int X_INITIAL_SPATIAL_GRID_SIZE = 16;

    static constexpr int Y_INITIAL_SPATIAL_GRID_SIZE = 16;

private:

    std::vector<GameObject *> gameObjects;

    Grids spatialGrids;

    int xSpaceSize;

    int ySpaceSize;

public:

    HierarchicalGrid(int xSpaceSize, int ySpaceSize);

    void addObject(GameObject *object);

    void detectAndHandleCollisions(CollisionHandler const &collisionHandler) const;

    void removeObject(GameObject *object);

    void updateGameObjectsAssociations();

private:

    void addObjectToSuitableSpatialGrid(GameObject *object);

    static bool canFitInto(GameObject const *object, int cellSize);

    static int getCellSizeForObject(GameObject const *gameObject);

    int getNextCellSize(int currentCellSize);

    int getPreviousCellSize(int currentCellSize);

    void createTailoredGridAndAddObjectToIt(GameObject *object, int cellSize);
};

#endif
