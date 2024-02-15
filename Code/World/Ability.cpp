#include "Ability.h"

Ability::Ability(Sprite* sprite, int x, int y)
    : sprite(sprite), x(x), y(y)
{
    getSpriteSize(sprite, width, height);
}

void Ability::draw(int playerY)
{
    drawSprite(sprite, x, y - playerY);
}

int Ability::getX()
{
    return x;
}

int& Ability::getY()
{
    return y;
}

int Ability::getWidth() 
{
    return width;
}

int Ability::getHeight() 
{
    return height;
}
