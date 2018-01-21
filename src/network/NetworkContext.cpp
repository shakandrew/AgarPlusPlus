#include "network/NetworkContext.h"

NetworkContext::NetworkContext() : nextId(1)
{

}

std::unordered_map<int, GameObject *> const &NetworkContext::getAllIdToObjectPairs() const
{
    return idToObject;
}

int NetworkContext::getIdForObject(GameObject *object)
{
    auto found = objectToId.find(object);
    if (found != objectToId.end()) {
        return found->second;
    } else {
        auto id = nextId;
        nextId += 1;
        registerObject(object, id);
        return id;
    }
}

GameObject *NetworkContext::getObjectForId(int id) const
{
    auto found = idToObject.find(id);
    if (found != idToObject.end()) {
        return found->second;
    }
    else {
        return nullptr;
    }
}

void NetworkContext::registerObject(GameObject *object, int id)
{
    idToObject[id] = object;
    objectToId[object] = id;
}

void NetworkContext::unregisterObject(GameObject *object)
{
    auto id = objectToId[object];
    objectToId.erase(object);
    idToObject.erase(id);
}
