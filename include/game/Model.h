#ifndef AGARPLUSPLUS_MODEL_H
#define AGARPLUSPLUS_MODEL_H

#include "GameObject.h"
#include <network/NetworkContext.h>

class Model
{
    std::vector<std::unique_ptr<GameObject>> gameObjects;

public:

    Model();

    void addObject(std::unique_ptr<GameObject> object);

    GameObject * createAndAddNewPlayerCell();

    void removeObject(GameObject *object);

};

#endif
