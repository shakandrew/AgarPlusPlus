#ifndef AGARPLUSPLUS_INPUT_H
#define AGARPLUSPLUS_INPUT_H

class Input {

    int angle;

    bool dirty;

    int speed;

public:

    Input(int angle, int speed);

    bool isDirty() const;

    void setAsClean();

};
#endif
