#pragma once
#include "Framework.h"

class Dragon 
{
public:
    Sprite* sprite;
    Dragon(Sprite* sprite, int x, int y, int jumpSpeed);
    void move(int speed);
    void stop();
    int width, height;
    int jumpSpeed;
    int moveSpeed;
    bool isPickedImmunity = false;
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();

private:
    int x, y;
};
