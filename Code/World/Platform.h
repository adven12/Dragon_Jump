#pragma once
#include "Framework.h"

class Platform 
{
public:
    Sprite* sprite;
    int x, y;
    int width, height;
    int platformMargin = 40;
    bool hasEnemy;
    bool isMoving = false;
    bool canHaveSomething = true;

    Platform(Sprite* sprite, int x, int y);
    void draw(int playerY);
};
