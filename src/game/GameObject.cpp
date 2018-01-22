#include <iostream>

#include "game/GameObject.h"

using std::vector;

GameObject::GameObject(int x, int y, int radius, ObjectType type, ObjectType interactionType,
                       bool selfCollidable, bool resizeable, bool nonDynamic)
    : associatedSpatialGrid{nullptr},
      cellIndex{0},
      extent{2 * radius},
      indexWithinCell{0},
      indexWithinHierarchicalGrid{0},
      indexWithinSpatialGrid{0},
      interactionType{interactionType},
      isMoveable{nonDynamic},
      isResizeable{resizeable},
      isSelfCollidable{selfCollidable},
      radius{radius},
      type{type},
      x{x}, xMax{x + radius}, xMin{x - radius},
      y{y}, yMax{y + radius}, yMin{y - radius}
{

}

size_t GameObject::getCellIndex() const
{
    return cellIndex;
}

int GameObject::getExtent() const
{
    return extent;
}

size_t GameObject::getIndexWithinCell() const
{
    return indexWithinCell;
}

std::size_t GameObject::getIndexWithinHierarchicalGrid() const
{
    return indexWithinHierarchicalGrid;
}

std::size_t GameObject::getIndexWithinSpatialGrid() const
{
    return indexWithinSpatialGrid;
}

ObjectType GameObject::getInteractionType() const
{
    return interactionType;
}

bool GameObject::getIsMoveable() const
{
    return isMoveable;
}

bool GameObject::getIsResizeable() const
{
    return isResizeable;
}

bool GameObject::getIsSelfCollidable() const
{
    return isSelfCollidable;
}

ObjectType GameObject::getType() const
{
    return type;
}

int GameObject::getXMin() const
{
    return xMin;
}

int GameObject::getYMin() const
{
    return yMin;
}

void GameObject::removeFromAssociatedSpatialGrid()
{
    associatedSpatialGrid->removeObject(this);
}

void GameObject::setAssociatedSpatialGrid(SpatialGrid *associatedSpatialGrid)
{
    GameObject::associatedSpatialGrid = associatedSpatialGrid;
}

void GameObject::setIndexWithinCell(size_t indexWithinCell)
{
    this->indexWithinCell = indexWithinCell;
}

void GameObject::setIndexWithinHierarchicalGrid(std::size_t indexWithinHierarchicalGrid)
{
    this->indexWithinHierarchicalGrid = indexWithinHierarchicalGrid;
}

void GameObject::setIndexWithinSpatialGrid(std::size_t indexWithinSpatialGrid)
{
    this->indexWithinSpatialGrid = indexWithinSpatialGrid;
}

bool GameObject::testForCollisionWithAABB(GameObject const &otherObject) const
{
    return (xMax >= otherObject.xMin) &&
           (xMin <= otherObject.xMax) &&
           (yMax >= otherObject.yMin) &&
           (yMin <= otherObject.yMax);
}

bool GameObject::updateWithinAssociatedSpatialGrid()
{
    return associatedSpatialGrid->update(this);
}

std::ostream &operator<<(std::ostream &os, GameObject const &object)
{
    return os << "[ [x: " << (object.xMin + object.xMax) / 2
              << ", y: " << (object.yMin + object.yMax) / 2
              << ", r: " << object.extent / 2
              << "], [x1: " << object.xMin
              << ", x2: " << object.xMax
              << "], [y1: " << object.yMin
              << ", y2: " << object.yMax
              << "] ]";
}

void GameObject::serialize(PacketDocument &document) const
{
    document.serializeHorizontalPosition(x);
    document.serializeVerticalPosition(y);
    document.serializeRadius(radius);
}

std::size_t GameObject::getIndexWithinGameModelContainer() const
{
    return indexWithinGameModelContainer;
}

void GameObject::setIndexWithinGameModelContainer(std::size_t indexWithinGameModelContainer)
{
    this->indexWithinGameModelContainer = indexWithinGameModelContainer;
}
