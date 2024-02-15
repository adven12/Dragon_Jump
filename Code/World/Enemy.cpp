#include "Enemy.h"

Enemy::Enemy(Sprite* sprite, int x, int y)
    : sprite(sprite), x(x), y(y)
{
    getSpriteSize(sprite, width, height);
}

void Enemy::draw(int playerY) 
{
    drawSprite(sprite, x-20, (y - playerY)-5);
}
