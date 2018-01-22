#include "grid/HierarchicalGrid.h"

constexpr int HierarchicalGrid::X_INITIAL_SPATIAL_GRID_SIZE;

constexpr int HierarchicalGrid::Y_INITIAL_SPATIAL_GRID_SIZE;

HierarchicalGrid::HierarchicalGrid(int xSpaceSize, int ySpaceSize)
        : gameObjects{},
          spatialGrids{SpatialGrid::hasLowerLevelThan},
          xSpaceSize{xSpaceSize},
          ySpaceSize{ySpaceSize}
{
    gameObjects.reserve(static_cast<std::size_t>(xSpaceSize * ySpaceSize * INITIAL_CELL_CAPACITY));
}

void HierarchicalGrid::addObject(GameObject *object)
{
    object->setIndexWithinHierarchicalGrid(gameObjects.size());
    gameObjects.push_back(object);
    addObjectToSuitableSpatialGrid(object);
}

void HierarchicalGrid::addObjectToSuitableSpatialGrid(GameObject *object)
{
    if (spatialGrids.empty()) {
        createTailoredGridAndAddObjectToIt(object, getCellSizeForObject(object));
    }
    else {
        auto currentCellSize = 0;
        auto currentGridIterator = spatialGrids.begin(), end = spatialGrids.end();
        while (currentGridIterator != end) {
            auto &currentGrid = *currentGridIterator;
            currentCellSize = currentGrid->getCellSize();
            if (canFitInto(object, currentCellSize)) {
                auto previousCellSize = getPreviousCellSize(currentCellSize);
                if (canFitInto(object, previousCellSize)) {
                    while (canFitInto(object, previousCellSize)) {
                        previousCellSize = getPreviousCellSize(previousCellSize);
                    }
                    createTailoredGridAndAddObjectToIt(object, getNextCellSize(previousCellSize));
                    return;
                }
                else {
                    auto nextGridIterator = std::next(currentGridIterator);
                    if (currentGrid->canStoreObject(object)) {
                        currentGrid->addObject(object);
                        return;
                    }
                    else if (( nextGridIterator == end) ||
                             ((*nextGridIterator)->getCellSize() > currentCellSize) ||
                             (currentGrid->isAboveObjectsGridIfInsideTheSameBucket(object))) {
                        createTailoredGridAndAddObjectToIt(object, currentCellSize);
                        return;
                    }
                }
            }
            ++currentGridIterator;
        }
        while (!canFitInto(object, currentCellSize)) {
            currentCellSize = static_cast<int>(currentCellSize * CELL_SIZE_SCALE_FACTOR);
        }
        createTailoredGridAndAddObjectToIt(object, currentCellSize);
    }
}

void HierarchicalGrid::createTailoredGridAndAddObjectToIt(GameObject *object, int cellSize)
{
    auto grid = std::make_unique<SpatialGrid>(X_INITIAL_SPATIAL_GRID_SIZE, Y_INITIAL_SPATIAL_GRID_SIZE,
                                              cellSize, object->getType(),
                                              object->getIsSelfCollidable());
    grid->addObject(object);
    spatialGrids.insert(std::move(grid));
}

int HierarchicalGrid::getCellSizeForObject(GameObject const *gameObject)
{
    return static_cast<int>(gameObject->getExtent() * CELL_SIZE_TO_OBJECT_AABB_EXTENT_RATIO);
}

bool HierarchicalGrid::canFitInto(GameObject const *object, int cellSize)
{
    return CELL_SIZE_TO_OBJECT_AABB_EXTENT_RATIO * object->getExtent() <= cellSize;
}

int HierarchicalGrid::getPreviousCellSize(int currentCellSize)
{
    return static_cast<int>(currentCellSize * CELL_SIZE_SCALE_FACTOR_INVERSE);
}

int HierarchicalGrid::getNextCellSize(int currentCellSize)
{
    return static_cast<int>(currentCellSize * CELL_SIZE_SCALE_FACTOR);
}

void HierarchicalGrid::removeObject(GameObject *object)
{
    auto lastObject = gameObjects.back();
    gameObjects.pop_back();
    if (object != lastObject)
    {
        auto objectIndex = object->getCellIndex();
        lastObject->setIndexWithinHierarchicalGrid(static_cast<std::size_t>(objectIndex));
        gameObjects[objectIndex] = lastObject;
    }
    object->removeFromAssociatedSpatialGrid();
}

void HierarchicalGrid::detectAndHandleCollisions(CollisionHandler const &collisionHandler) const
{
    auto end = spatialGrids.end();
    for (auto &grid : spatialGrids) {
        grid->detectAndHandleCollisionsWithin(collisionHandler);
        grid->detectAndHandleCollisionsAgainstHigherGrids(spatialGrids.upper_bound(grid), end, collisionHandler);
    }
}

void HierarchicalGrid::updateGameObjectsAssociations()
{
    for (auto object : gameObjects) {
        if (!object->getIsMoveable() || object->getIsResizeable()) {
            if (!object->updateWithinAssociatedSpatialGrid()) {
                object->removeFromAssociatedSpatialGrid();
                addObjectToSuitableSpatialGrid(object);
            }
        }
    }
}
