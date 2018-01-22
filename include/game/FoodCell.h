//
// Created by koshachok on 1/22/18.
//

#include "GameObject.h"

#ifndef AGARPLUSPLUS_CELL_H
#define AGARPLUSPLUS_CELL_H

#endif //AGARPLUSPLUS_CELL_H


class FoodCell : public GameObject {
private:

public:
    FoodCell(int x, int y, int radius) : GameObject(x, y, radius, ObjectType::FOOD,
                                                    ObjectType::PLAYER | ObjectType::RANGE, false, false, false) {};
};