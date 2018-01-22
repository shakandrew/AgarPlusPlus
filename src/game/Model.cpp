#include <game/Model.h>
#include <game/PlayerCell.h>
#include <network/Input.h>
#include <cmath>
#include <iostream>
#define PI 3.14

Model::Model() {

}

void Model::addObject(std::unique_ptr<GameObject> object) {
    object->setIndexWithinGameModelContainer(gameObjects.size());
    gameObjects.push_back(std::move(object));
}

GameObject *Model::createAndAddNewPlayerCell() {
    int x = 100000; /* roll x */
    int y = 100000; /* roll y */
    int r = 100000; /* roll r */
    auto object = std::make_unique<PlayerCell>(x, y, r);
    auto objectPointer = object.get();
    addObject(std::move(object));
    return objectPointer;
}

void Model::moveObject(GameObject* obj, Input *input) {
    if (input)
        std::cout<<"YES";
    else
        std::cout<<"NO";
    auto x = obj->getX();
    x += round(cos(double(input->angle) / 360 * PI) * input->speed);
    obj->setX(x);
    auto y = obj->getY();
    y += round(sin(double(input->angle) / 360 * PI) * input->speed);
    obj->setX(x);
    obj->setY(y);

}

void Model::removeObject(GameObject *object) {
    auto lastObject = std::move(gameObjects.back());
    gameObjects.pop_back();
    if (object != lastObject.get()) {
        auto objectIndex = object->getIndexWithinGameModelContainer();
        lastObject->setIndexWithinGameModelContainer(objectIndex);
        gameObjects[objectIndex] = std::move(lastObject);
    }
}
