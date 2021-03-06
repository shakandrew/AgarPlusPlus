/*!
 * \author Mateusz Mobieszek
 * \version 0.1
 * \date 2018-01-21
 * \copyright GNU Public License
 * \mainpage LOL
 */
#ifndef AGARPLUSPLUS_GAMEOBJECT_H
#define AGARPLUSPLUS_GAMEOBJECT_H

#include <vector>

#include "game/ObjectType.h"
#include "grid/SpatialGrid.h"
#include "network/PacketDocument.h"


class PacketDocument;
class SpatialGrid;

class GameObject {

private:

    SpatialGrid* associatedSpatialGrid;

    std::size_t cellIndex;

    int extent;

    std::size_t indexWithinCell;

    std::size_t indexWithinGameModelContainer;

    std::size_t indexWithinHierarchicalGrid;

    std::size_t indexWithinSpatialGrid;

    ObjectType interactionType;

    bool isMoveable;

    bool isResizeable;

    bool isSelfCollidable;

    int radius;
public:
    int getRadius() const;

    void setRadius(int radius);

private:

    ObjectType type;

    int x;
public:
    int getX() const;

    void setX(int x);

private:

    int xMax;

    int xMin;

    int y;
public:
    int getY() const;

    void setY(int y);

private:

    int yMax;

    int yMin;

public:

    GameObject(int x, int y, int radius, ObjectType type = ObjectType::PLAYER,
               ObjectType interactionType = ObjectType::PLAYER, bool selfCollidable = true,
               bool resizeable = true, bool moveable = true);

    size_t getCellIndex() const;

    int getExtent() const;

    std::size_t getIndexWithinCell() const;

    std::size_t getIndexWithinHierarchicalGrid() const;

    std::size_t getIndexWithinSpatialGrid() const;

    std::size_t getIndexWithinGameModelContainer() const;

    ObjectType getInteractionType() const;

    bool getIsMoveable() const;

    bool getIsResizeable() const;

    bool getIsSelfCollidable() const;

    ObjectType getType() const;

    int getXMin() const;

    int getYMin() const;

    void removeFromAssociatedSpatialGrid();

    void serialize(PacketDocument &document) const;

    void setAssociatedSpatialGrid(SpatialGrid *associatedSpatialGrid);

    void setIndexWithinCell(std::size_t indexWithinCell);

    void setIndexWithinHierarchicalGrid(std::size_t indexWithinHierarchicalGrid);

    void setIndexWithinSpatialGrid(std::size_t indexWithinSpatialGrid);

    void setIndexWithinGameModelContainer(std::size_t indexWithinGameModelContainer);

    bool testForCollisionWithAABB(GameObject const &otherObject) const;

    bool updateWithinAssociatedSpatialGrid();

    friend std::ostream &operator<<(std::ostream &os, GameObject const &object);
};

#endif
