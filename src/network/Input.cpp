#include "network/Input.h"

Input::Input(int angle, int speed)
    : angle(angle),
      dirty(true),
      speed(speed)
{

}

bool Input::isDirty() const
{
    return dirty;
}

void Input::setAsClean()
{
    dirty = false;
}
