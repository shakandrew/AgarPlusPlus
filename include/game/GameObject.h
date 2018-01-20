#ifndef AGAR_GAMEOBJECT_H
#define AGAR_GAMEOBJECT_H

#include <vector>

#include "game/ObjectType.h"
#include "grid/SpatialGrid.h"

class SpatialGrid;

class GameObject {

private:

    SpatialGrid* associatedSpatialGrid;

    std::size_t cellIndex;

    int extent;

    int id;

    std::size_t indexWithinCell;

    std::size_t indexWithinHierarchicalGrid;

    std::size_t indexWithinSpatialGrid;

    ObjectType interactionType;

    bool isMoveable;

    bool isResizeable;

    bool isSelfCollidable;

    ObjectType type;

    int xMax;

    int xMin;

    int yMax;

    int yMin;

public:

    GameObject(int id, int x, int y, int radius, ObjectType type = ObjectType::PLAYER,
               ObjectType interactionType = ObjectType::PLAYER, bool selfCollidable = true,
               bool resizeable = true, bool moveable = true);

    size_t getCellIndex() const;

    int getExtent() const;

    std::size_t getIndexWithinCell() const;

    std::size_t getIndexWithinHierarchicalGrid() const;

    std::size_t getIndexWithinSpatialGrid() const;

    ObjectType getInteractionType() const;

    bool getIsMoveable() const;

    bool getIsResizeable() const;

    bool getIsSelfCollidable() const;

    ObjectType getType() const;

    int getXMin() const;

    int getYMin() const;

    void removeFromAssociatedSpatialGrid();

    void setAssociatedSpatialGrid(SpatialGrid *associatedSpatialGrid);

    void setIndexWithinCell(std::size_t indexWithinCell);

    void setIndexWithinHierarchicalGrid(std::size_t indexWithinHierarchicalGrid);

    void setIndexWithinSpatialGrid(std::size_t indexWithinSpatialGrid);

    bool testForCollisionWithAABB(GameObject const &otherObject) const;

    bool updateWithinAssociatedSpatialGrid();

    friend std::ostream &operator<<(std::ostream &os, GameObject const &object);
};

#endif