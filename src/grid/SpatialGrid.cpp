#include <stdexcept>

#include "grid/HierarchicalGrid.h"
#include "grid/SpatialGrid.h"

SpatialGrid::SpatialGrid(int xGridSize, int yGridSize, int cellSize, ObjectType storageMask, bool holdsSelfCollidable)
        : adjacencyOffsetsForInnerCells{{-(xGridSize + 1),  -(xGridSize),   -(xGridSize - 1),
                                         -1,                0,              1,
                                         (xGridSize - 1),   xGridSize,      (xGridSize + 1)}},
          cellCount{(xGridSize) * (yGridSize)},
          cellSize{cellSize},
          cells{cellCount, GridCell(adjacencyOffsetsForInnerCells.data(), cellSize)},
          gameObjects{},
          holdsSelfCollidable{holdsSelfCollidable},
          occupiedCells{},
          storageType{storageMask},
          xGridSize{xGridSize},
          yGridSize{yGridSize}
{
    if (!isPowerOfTwo(xGridSize) || !isPowerOfTwo(yGridSize)) {
        throw std::invalid_argument("xGridSize and yGridSize must be both powers of 2.");
    }

    if (xGridSize < X_MIN_SPATIAL_GRID_COORD || yGridSize < Y_MIN_SPATIAL_GRID_COORD) {
        throw std::invalid_argument("xMaxCellCoord and yMaxCellCoord must be both greater than or equal to 4.");
    }

    gameObjects.reserve(static_cast<std::size_t>(cellCount * HierarchicalGrid::INITIAL_CELL_CAPACITY));
    occupiedCells.reserve(static_cast<std::size_t>(cellCount / GRID_DENSITY_RESIZE_THRESHOLD) + 1);
    setupAdjacencyOffsetsOfOuterCells();
}

bool SpatialGrid::isPowerOfTwo(int number) const
{
    return number && !(number & (number - 1));
}

void SpatialGrid::setupAdjacencyOffsetsOfOuterCells()
{
    SpatialGrid::OuterCellAdjacentCells adjacencyOffsets = nullptr;
    for (auto xCellCoord = 0; xCellCoord < xGridSize; ++xCellCoord) {
        adjacencyOffsets = computeAdjacencyOffsetsForOuterCell(xCellCoord, 0);
        (cells[xCellCoord]).setOuterCellAdjacentCellsOffsets(std::move(adjacencyOffsets));
        adjacencyOffsets = computeAdjacencyOffsetsForOuterCell(xCellCoord, yGridSize - 1);
        cells[(yGridSize - 1) * xGridSize + xCellCoord].setOuterCellAdjacentCellsOffsets(std::move(adjacencyOffsets));
    }
    for (auto yCellCoord = 1; yCellCoord < yGridSize; ++yCellCoord) {
        adjacencyOffsets = computeAdjacencyOffsetsForOuterCell(0, yCellCoord);
        cells[yCellCoord * xGridSize].setOuterCellAdjacentCellsOffsets(std::move(adjacencyOffsets));
        adjacencyOffsets = computeAdjacencyOffsetsForOuterCell(xGridSize - 1, yCellCoord);
        cells[yCellCoord * xGridSize + (xGridSize - 1)].setOuterCellAdjacentCellsOffsets(std::move(adjacencyOffsets));
    }
}

SpatialGrid::OuterCellAdjacentCells SpatialGrid::computeAdjacencyOffsetsForOuterCell(int xCellCoord, int yCellCoord) const
{
    auto adjacencyOffsets = std::make_unique<std::array<int, SpatialGrid::ADJACENT_CELLS_COUNT>>();
    auto xModMask = xGridSize - 1;
    auto yModMask = yGridSize - 1;
    (*adjacencyOffsets)[THIS_CELL_OFFSET] = 0;
    (*adjacencyOffsets)[W_CELL_OFFSET] = -(xCellCoord - ((xCellCoord - 1) & xModMask));
    (*adjacencyOffsets)[E_CELL_OFFSET] = -(xCellCoord - ((xCellCoord + 1) & xModMask));
    (*adjacencyOffsets)[N_CELL_OFFSET] = -((yCellCoord - ((yCellCoord + 1) & yModMask)) * yGridSize);
    (*adjacencyOffsets)[S_CELL_OFFSET] = -((yCellCoord - ((yCellCoord - 1) & yModMask)) * yGridSize);
    (*adjacencyOffsets)[SW_CELL_OFFSET] = (*adjacencyOffsets)[W_CELL_OFFSET] + (*adjacencyOffsets)[S_CELL_OFFSET];
    (*adjacencyOffsets)[SE_CELL_OFFSET] = (*adjacencyOffsets)[E_CELL_OFFSET] + (*adjacencyOffsets)[S_CELL_OFFSET];
    (*adjacencyOffsets)[NW_CELL_OFFSET] = (*adjacencyOffsets)[W_CELL_OFFSET] + (*adjacencyOffsets)[N_CELL_OFFSET];
    (*adjacencyOffsets)[NE_CELL_OFFSET] = (*adjacencyOffsets)[E_CELL_OFFSET] + (*adjacencyOffsets)[N_CELL_OFFSET];
    return adjacencyOffsets;
}

void SpatialGrid::GridCell::setOuterCellAdjacentCellsOffsets(SpatialGrid::OuterCellAdjacentCells adjacentCellsOffsets)
{
    outerCellAdjacentCellsOffsets = std::move(adjacentCellsOffsets);
}

void SpatialGrid::addObject(GameObject *object)
{
    if (!canStoreObject(object)) {
        throw std::invalid_argument("Object type be compatible with grid storage type.");
    }
    object->setIndexWithinSpatialGrid(gameObjects.size());
    object->setAssociatedSpatialGrid(this);
    gameObjects.push_back(object);
    auto cellIndex = computeCellIndex(object->getXMin(), object->getYMin());
    auto &cell = cells[cellIndex];
    if (cell.isEmpty()) {
        cell.setIndexInOccupiedCellsContainer(occupiedCells.size());
        occupiedCells.push_back(&cell);
    }
    cell.addObject(object);
    if (wasDensityThresholdExceeded()) {
        resize();
    }
}

bool SpatialGrid::canStoreObject(GameObject const *object) const
{
    return (storageType == object->getType());
}

bool SpatialGrid::wasDensityThresholdExceeded() const
{
    return (cellCount / gameObjects.size()) < GRID_DENSITY_RESIZE_THRESHOLD;
}

void SpatialGrid::resize()
{
    std::vector<GameObject *> removedObjects;
    removedObjects.reserve(static_cast<std::size_t>(gameObjects.size()));
    for (auto &gridCell : occupiedCells) {
        auto objectsFromCell = gridCell->getGameObjects();
        std::move(objectsFromCell->begin(), objectsFromCell->end(), std::back_inserter(removedObjects));
    }
    occupiedCells.clear();
    cells.clear();
    xGridSize *= GRID_RESIZE_SCALE_FACTOR;
    yGridSize *= GRID_RESIZE_SCALE_FACTOR;
    cellCount = (xGridSize) * (xGridSize);
    cells.resize(static_cast<std::size_t>(cellCount), GridCell(adjacencyOffsetsForInnerCells.data(), cellSize));
    occupiedCells.reserve(static_cast<std::size_t>(cellCount / GRID_DENSITY_RESIZE_THRESHOLD) + 1);
    setupAdjacencyOffsetsOfOuterCells();
    for (auto &removedObject : removedObjects) {
        auto cellIndex = computeCellIndex(removedObject->getXMin(), removedObject->getYMin());
        auto &cell = cells[cellIndex];
        cell.addObject(removedObject);
        cell.setIndexInOccupiedCellsContainer(occupiedCells.size());
        occupiedCells.push_back(&cell);
    }
}

std::vector<GameObject *>* SpatialGrid::GridCell::getGameObjects() const
{
    return gameObjects.get();
}

bool SpatialGrid::update(GameObject *object)
{
    if (hasSuitableCellSizeForObject(object)) {
        auto newObjectCellIndex = computeCellIndex(object->getXMin(), object->getYMin());
        auto oldObjectCellIndex = object->getCellIndex();
        if (newObjectCellIndex != oldObjectCellIndex) {
            cells[oldObjectCellIndex].removeObject(object);
            cells[newObjectCellIndex].addObject(object);
        }
        return true;
    } else {
        return false;
    }
}

bool SpatialGrid::hasSuitableCellSizeForObject(const GameObject *object) const
{
    auto objectExtent = object->getExtent();
    auto ratio = HierarchicalGrid::CELL_SIZE_TO_OBJECT_AABB_EXTENT_RATIO;
    return ((objectExtent * ratio * HierarchicalGrid::CELL_SIZE_SCALE_FACTOR) > cellSize) &&
           ((objectExtent * ratio) <= cellSize);
}

std::size_t SpatialGrid::computeCellIndex(int xWorldPos, int yWorldPos) const
{
    int xGridCellCoord = xWorldPos / cellSize;
    int yGridCellCoord = yWorldPos / cellSize;
    auto xHash = computeGridCellCoordHash(xGridCellCoord, xGridSize);
    auto yHash = computeGridCellCoordHash(yGridCellCoord, yGridSize);
    return static_cast<std::size_t>((yHash * xGridSize) + xHash);
}

int SpatialGrid::computeGridCellCoordHash(int gridCellCoord, int gridSize) const
{
    auto modMask = gridSize - 1;
    auto hash = 0;
    if (gridCellCoord < 0) {
        hash = gridSize - 1 - (gridCellCoord & modMask);
    }
    else {
        hash = gridCellCoord & modMask;
    }
    return hash;
}

void SpatialGrid::removeObject(GameObject *object)
{
    auto lastObject = gameObjects.back();
    gameObjects.pop_back();
    if (object != lastObject) {
        auto objectIndex = object->getCellIndex();
        lastObject->setIndexWithinSpatialGrid(static_cast<std::size_t>(objectIndex));
        gameObjects[objectIndex] = lastObject;
    }
    auto cellIndex = object->getCellIndex();
    auto &cell = cells[cellIndex];
    cell.removeObject(object);
    if (cell.isEmpty()) {
        auto lastCell = occupiedCells.back();
        occupiedCells.pop_back();
        if (&cell != lastCell) {
            lastCell->setIndexInOccupiedCellsContainer(static_cast<std::size_t>(cellIndex));
            occupiedCells[cellIndex] = lastCell;
        }
    }
    object->setAssociatedSpatialGrid(nullptr);
}

void SpatialGrid::GridCell::setIndexInOccupiedCellsContainer(std::size_t indexInOccupiedCellsContainer)
{
    this->indexInOccupiedCellsContainer = indexInOccupiedCellsContainer;
}

void SpatialGrid::detectAndHandleCollisionsWithin(CollisionHandler const &collisionHandler) const
{
    if (holdsSelfCollidable) {
        for (auto cell : occupiedCells) {
            cell->testObjectsWithinForCollisions(collisionHandler);
        }
    }
}

void SpatialGrid::detectAndHandleCollisionsAgainstHigherGrids(Grids::iterator begin,
                                                              Grids::iterator end,
                                                              CollisionHandler const &collisionHandler) const
{
    for (auto object : gameObjects) {
        for (auto &higherSpatialGridIterator = begin; higherSpatialGridIterator != end; ++higherSpatialGridIterator) {
            auto &higherSpatialGrid = *higherSpatialGridIterator;
            if ((higherSpatialGrid->storageType & object->getInteractionType()) != ObjectType::NONE) {
                auto correspondingCellIndex = higherSpatialGrid->computeCellIndex(object->getXMin(), object->getYMin());
                auto &correspondingCell  = higherSpatialGrid->cells[correspondingCellIndex];
                correspondingCell.testObjectsWithinForCollisionsAgainstObject(object, collisionHandler);
            }
        }
    }
}

bool SpatialGrid::hasLowerLevelThan(std::unique_ptr<SpatialGrid> const &first, std::unique_ptr<SpatialGrid> const &second)
{
    return (first->cellSize < second->cellSize) ? true :
           (first->cellSize != second->cellSize) ? false :
           (first->storageType < second->storageType);
}

int SpatialGrid::getCellSize() const
{
    return cellSize;
}

bool SpatialGrid::isAboveObjectsGridIfInsideTheSameBucket(GameObject const *gameObject) const
{
    return (storageType > gameObject->getType());
}

SpatialGrid::GridCell::GridCell(int *adjacencyOffsets, int cellSize)
    : adjacentCellsOffsets{adjacencyOffsets},
      gameObjects{nullptr},
      indexInOccupiedCellsContainer{0},
      outerCellAdjacentCellsOffsets{nullptr},
      size{cellSize}
{

}

SpatialGrid::GridCell::GridCell(SpatialGrid::GridCell const &gridCell)
    : adjacentCellsOffsets{gridCell.adjacentCellsOffsets},
      gameObjects{nullptr},
      indexInOccupiedCellsContainer{0},
      outerCellAdjacentCellsOffsets{nullptr},
      size{gridCell.size}
{

}

SpatialGrid::GridCell& SpatialGrid::GridCell::operator=(SpatialGrid::GridCell const &gridCell)
{
    adjacentCellsOffsets = gridCell.adjacentCellsOffsets;
    gameObjects = nullptr;
    indexInOccupiedCellsContainer = gridCell.indexInOccupiedCellsContainer;
    outerCellAdjacentCellsOffsets = nullptr;
    return *this;
}

void SpatialGrid::GridCell::addObject(GameObject *object)
{
    if (gameObjects == nullptr) {
        gameObjects = std::make_unique<std::vector<GameObject *>>();
        gameObjects->reserve(static_cast<std::size_t>(HierarchicalGrid::INITIAL_CELL_CAPACITY));
    }
    object->setIndexWithinCell(gameObjects->size());
    gameObjects->push_back(object);
}

void SpatialGrid::GridCell::removeObject(GameObject *object)
{
    auto lastObject = gameObjects->back();
    gameObjects->pop_back();
    if (object != lastObject) {
        auto objectIndex = object->getIndexWithinCell();
        lastObject->setIndexWithinCell(objectIndex);
        (*gameObjects)[objectIndex] = lastObject;
    }
    if (gameObjects->empty()) {
        gameObjects = nullptr;
    }
}

void SpatialGrid::GridCell::testObjectsWithinForCollisions(CollisionHandler const &collisionHandler)
{
    for (auto first = gameObjects->begin(), end = gameObjects->end(); first != end; ++first) {
        for (auto second = std::next(first); second != end; ++second) {
            if ((*first)->testForCollisionWithAABB(**second)) {
                collisionHandler(*first, *second);
            }
        }
        bool overlapsEast = doesObjectOverlapAdjacentCell(*first, E_CELL_OFFSET);
        if (overlapsEast) {
            testObjectForCollisionAgainstObjectsInAdjacentCell(*first, E_CELL_OFFSET, collisionHandler);
        }
        if (doesObjectOverlapAdjacentCell(*first, S_CELL_OFFSET)) {
            testObjectForCollisionAgainstObjectsInAdjacentCell(*first, SW_CELL_OFFSET, collisionHandler);
            testObjectForCollisionAgainstObjectsInAdjacentCell(*first, S_CELL_OFFSET, collisionHandler);
            if (overlapsEast) {
                testObjectForCollisionAgainstObjectsInAdjacentCell(*first, SE_CELL_OFFSET, collisionHandler);
            }
        }
    }
}

bool SpatialGrid::GridCell::doesObjectOverlapAdjacentCell(GameObject const *object, int offsetIndex) const
{
    int extent = object->getExtent();
    switch (offsetIndex) {
        case E_CELL_OFFSET:
            return (object->getXMin() % size) + extent >= size;
        case S_CELL_OFFSET:
            return (object->getYMin() % size) + extent >= size;
        default:
            return true;
    }
}

void SpatialGrid::GridCell::testObjectForCollisionAgainstObjectsInAdjacentCell(GameObject *gameObject, int offsetIndex,
                                                                               CollisionHandler const &collisionHandler)
{
    GridCell *adjacentGridCell = nullptr;
    if (outerCellAdjacentCellsOffsets) {
        adjacentGridCell = this + (*outerCellAdjacentCellsOffsets)[offsetIndex];
    } else {
        adjacentGridCell = this + adjacentCellsOffsets[offsetIndex];
    }
    if (!adjacentGridCell->isEmpty()) {
        for (auto &objectFromAdjacentCell : *(adjacentGridCell->gameObjects)) {
            if(gameObject->testForCollisionWithAABB(*objectFromAdjacentCell)) {
                collisionHandler(gameObject, objectFromAdjacentCell);
            }
        }
    }
}

void SpatialGrid::GridCell::testObjectsWithinForCollisionsAgainstObject(GameObject *object,
                                                                        CollisionHandler const &collisionHandler)
{
    for (auto offsetIndex = 0; offsetIndex < ADJACENT_CELLS_COUNT; ++offsetIndex) {
        GridCell *adjacentGridCell = nullptr;
        if (outerCellAdjacentCellsOffsets) {
            adjacentGridCell = this + (*outerCellAdjacentCellsOffsets)[offsetIndex];
        } else {
            adjacentGridCell = this + adjacentCellsOffsets[offsetIndex];
        }
        if (!adjacentGridCell->isEmpty()) {
            for (auto &other : *(adjacentGridCell->gameObjects)) {
                if (object->testForCollisionWithAABB(*other)) {
                    collisionHandler(object, other);
                }
            }
        }
    }
}

bool SpatialGrid::GridCell::isEmpty() const
{
    return (gameObjects == nullptr) || gameObjects->empty();
}
