//
// Created by koshachok on 1/22/18.
//

#include "GameObject.h"

#ifndef AGARPLUSPLUS_PLAYERCELL_H
#define AGARPLUSPLUS_PLAYERCELL_H

#endif //AGARPLUSPLUS_PLAYERCELL_H

class PlayerCell : public GameObject {
private:

public:
    PlayerCell(int x, int y, int radius) : GameObject(x, y, radius, ObjectType::PLAYER,
                                                      ObjectType::PLAYER | ObjectType::RANGE | ObjectType::BOOST |
                                                      ObjectType::FOOD, true, true, true) {};
};