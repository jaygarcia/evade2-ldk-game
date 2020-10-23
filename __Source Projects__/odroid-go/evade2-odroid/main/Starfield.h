#ifndef STARFIELD_H
#define STARFIELD_H
// Based off of: https://github.com/ankorite/Arduboy-Starfield-Demo

#include "Evade2.h"



class Star {
public:
    void randomize(int xMin, int xMax, int yMin, int yMax, int zMin, int zMax, int speedMin, int speedMax);
    float x, y, z;
    int screen_x, screen_y, old_screen_x, old_screen_y, speed;
};

class Starfield {
protected:
    static void initStar(int i);

public:
    static int curSpeed;
    static int minSpeed;
    static BOOL boostSpeed;
    static BOOL warp;
    static void init();
    static void render();
};

#endif
