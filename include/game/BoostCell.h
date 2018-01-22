//
// Created by koshachok on 1/22/18.
//

#include "ObjectType.h"
#include "GameObject.h"

#ifndef AGARPLUSPLUS_BOOSTCELL_H
#define AGARPLUSPLUS_BOOSTCELL_H

#endif //AGARPLUSPLUS_BOOSTCELL_H

class BoostCell : public GameObject {
private:

public:
    BoostCell(int x, int y, int radius) : GameObject(x, y, radius, ObjectType::BOOST,
                                                    ObjectType::PLAYER | ObjectType::RANGE, false, false, false) {};
};