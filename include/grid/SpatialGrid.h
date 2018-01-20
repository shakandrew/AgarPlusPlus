#ifndef AGAR_SPATIALGRID_H
#define AGAR_SPATIALGRID_H

#include <array>
#include <memory>
#include <vector>

#include "game/GameObject.h"
#include "grid/GridsTypes.h"

class SpatialGrid {

public:

    static constexpr int ADJACENT_CELLS_COUNT = 9;

    using InnerCellAdjacentCells = std::array<int, ADJACENT_CELLS_COUNT>;

    using OuterCellAdjacentCells = std::unique_ptr<InnerCellAdjacentCells>;

    static constexpr std::size_t SW_CELL_OFFSET = 0;

    static constexpr std::size_t S_CELL_OFFSET = 1;

    static constexpr std::size_t SE_CELL_OFFSET = 2;

    static constexpr std::size_t W_CELL_OFFSET = 3;

    static constexpr std::size_t THIS_CELL_OFFSET = 4;

    static constexpr std::size_t E_CELL_OFFSET = 5;

    static constexpr std::size_t NW_CELL_OFFSET = 6;

    static constexpr std::size_t N_CELL_OFFSET = 7;

    static constexpr std::size_t NE_CELL_OFFSET = 8;

    static constexpr double GRID_DENSITY_RESIZE_THRESHOLD = 10;

    static constexpr int GRID_RESIZE_SCALE_FACTOR = 2;

    static constexpr int X_MIN_SPATIAL_GRID_COORD = 4;

    static constexpr int Y_MIN_SPATIAL_GRID_COORD = 4;

private:

    class GridCell;

    InnerCellAdjacentCells adjacencyOffsetsForInnerCells;

    int cellCount;

    int cellSize;

    std::vector<GridCell> cells;

    std::vector<GameObject *> gameObjects;

    bool holdsSelfCollidable;

    std::vector<GridCell *> occupiedCells;

    ObjectType storageType;

    int xGridSize;

    int yGridSize;

public:

    SpatialGrid(int xGridSize, int yGridSize, int cellSize, ObjectType storageMask, bool holdsSelfCollidable);

    void addObject(GameObject * object);

    bool canStoreObject(GameObject const *object) const;

    int computeCellIndex(int xWorldPos, int yWorldPos) const;

    int computeGridCellCoordHash(int gridCellCoord, int gridSize) const;

    void detectAndHandleCollisionsAgainstHigherGrids(Grids::iterator &begin, Grids::iterator &end,
                                                     CollisionHandler const &collisionHandler) const;

    void detectAndHandleCollisionsWithin(CollisionHandler const &collisionHandler) const;

    int getCellSize() const;

    static bool hasLowerLevelThan(std::unique_ptr<SpatialGrid> const &first, std::unique_ptr<SpatialGrid> const &second);

    bool isAboveObjectsGridIfInsideTheSameBucket(GameObject const *gameObject) const;

    void removeObject(GameObject * object);

    bool update(GameObject *object);

private:

    OuterCellAdjacentCells computeAdjacencyOffsetsForOuterCell(int xCellCoord, int yCellCoord) const;

    bool hasSuitableCellSizeForObject(GameObject const *object) const;

    bool isPowerOfTwo(int number) const;

    void resize();

    void setupAdjacencyOffsetsOfOuterCells();

    bool wasDensityThresholdExceeded() const;

    class GridCell {

        int *adjacentCellsOffsets;

        std::unique_ptr<std::vector<GameObject *>> gameObjects;

        std::size_t indexInOccupiedCellsContainer;

        OuterCellAdjacentCells outerCellAdjacentCellsOffsets;

        int size;

    public:

        GridCell(int *adjacencyOffsets, int cellSize);

        GridCell(GridCell const &gridCell);

        GridCell &operator=(const GridCell &gridCell);

        void addObject(GameObject *object);

        std::vector<GameObject *>* getGameObjects() const;

        bool isEmpty() const;

        void removeObject(GameObject *object);

        void setIndexInOccupiedCellsContainer(std::size_t indexInOccupiedCellsContainer);

        void setOuterCellAdjacentCellsOffsets(OuterCellAdjacentCells adjacentCellsOffsets);

        void testObjectsWithinForCollisions(CollisionHandler const &collisionHandler);

        void testObjectsWithinForCollisionsAgainstObject(GameObject *object, CollisionHandler const &collisionHandler);

    private:

        bool doesObjectOverlapAdjacentCell(GameObject const *object, int offsetIndex) const;

        void testObjectForCollisionAgainstObjectsInAdjacentCell(GameObject *gameObject, int offsetIndex,
                                                                CollisionHandler const &collisionHandler);
    };
};

#endif
