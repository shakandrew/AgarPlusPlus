//
// Created by koshachok on 1/22/18.
//

#include "GameObject.h"

#ifndef AGARPLUSPLUS_RANGE_H
#define AGARPLUSPLUS_RANGE_H

#endif //AGARPLUSPLUS_RANGE_H

class Range : public GameObject {
private:

public:
    Range(int x, int y, int radius) : GameObject(x, y, radius, ObjectType::RANGE,
                                                 ObjectType::PLAYER | ObjectType::FOOD | ObjectType::BOOST, false,
                                                 true, true) {};
};