#include "Dragon.h"

Dragon::Dragon(Sprite* sprite, int x, int y, int jumpSpeed)
    : sprite(sprite), x(x), y(y), jumpSpeed(jumpSpeed), moveSpeed(0) 
{
    getSpriteSize(sprite, width, height);
}

void Dragon::move(int speed) 
{
    moveSpeed = speed;
}

void Dragon::stop() 
{
    moveSpeed = 0;
}

void Dragon::setX(int x)
{
    this->x = x;
}

void Dragon::setY(int y)
{
    this->y = y;
}

int Dragon::getX()
{
    return x;
}

int Dragon::getY()
{
    return y;
}
