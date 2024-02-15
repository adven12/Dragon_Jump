#pragma once
#include "Framework.h"

class Ability 
{
public:
    Ability(Sprite* sprite, int x, int y);
    void draw(int playerY);
    Sprite* sprite;

    int getX();
    int& getY();
    int getWidth();
    int getHeight();

private:
    int x, y;
    int width, height;
};