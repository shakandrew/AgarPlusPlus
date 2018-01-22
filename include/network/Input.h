#ifndef AGARPLUSPLUS_INPUT_H
#define AGARPLUSPLUS_INPUT_H

class Input {



    bool dirty;



public:

    Input(int angle, int speed);

    bool isDirty() const;

    void setAsClean();
    int speed;
    int angle;
};
#endif
