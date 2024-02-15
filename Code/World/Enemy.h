#pragma once
#include "Framework.h"

class Enemy 
{
public:
    Sprite* sprite;
    int x, y;
    int width, height;

    Enemy(Sprite* sprite, int x, int y);
    void draw(int playerY);
};
