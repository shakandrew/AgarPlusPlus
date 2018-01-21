#ifndef AGARPLUSPLUS_NETWORKCONTEXT_H
#define AGARPLUSPLUS_NETWORKCONTEXT_H

#include <unordered_map>

#include "game/GameObject.h"

class NetworkContext
{
    int nextId;

    std::unordered_map<int, GameObject *> idToObject;

    std::unordered_map<GameObject *, int> objectToId;

public:

    NetworkContext();

    std::unordered_map<int, GameObject *> const& getAllIdToObjectPairs() const;

    int getIdForObject(GameObject *object);

    GameObject* getObjectForId(int id) const;

    void registerObject(GameObject *object, int id);

    void unregisterObject(GameObject *object);
};

#endif
