#include <game/Model.h>
#include <game/PlayerCell.h>

Model::Model()
{

}

void Model::addObject(std::unique_ptr<GameObject> object)
{
    object->setIndexWithinGameModelContainer(gameObjects.size());
    gameObjects.push_back(std::move(object));
}

GameObject* Model::createAndAddNewPlayerCell()
{
    int x = 0; /* roll x */
    int y = 0; /* roll y */
    int r = 0; /* roll r */
    auto object = std::make_unique<PlayerCell>(x, y, r);
    auto objectPointer = object.get();
    addObject(std::move(object));
    return objectPointer;
}

void Model::removeObject(GameObject *object)
{
    auto lastObject = std::move(gameObjects.back());
    gameObjects.pop_back();
    if (object != lastObject.get()) {
        auto objectIndex = object->getIndexWithinGameModelContainer();
        lastObject->setIndexWithinGameModelContainer(objectIndex);
        gameObjects[objectIndex] = std::move(lastObject);
    }
}
