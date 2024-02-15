#pragma once
#include "Framework.h"
#include <../World/GameObjects.h>
#include <iostream>
#include <list>
#include <random>
#include <algorithm>

const long double  M_PI = 3.14159265358979323846;

class MyFramework : public Framework
{
public:

    virtual void PreInit(int& width, int& height, bool& fullScreen);

    virtual bool Init();

    virtual void Close();

    virtual bool Tick();

    virtual void onMouseMove(int x, int y, int xRelative, int yRelative);

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased);

    virtual void onKeyPressed(FRKey k);

    virtual void onKeyReleased(FRKey k);

    virtual const char* GetTitle() override;

    void updateCamera();

    void firstGenPlatforms();

    void handleCollisions();

    void handleProjectiles();

    bool checkHealth();

    static void HandleExeption(const std::exception& exe);

private:
    int width, height; // window size 

    Dragon* dragon;  // player
    Sprite* playerSprite;

    int numberLives = 5;
    unsigned long long momentPickedImmunity = 0;

    const int gravity = 5;

    // will calculate time between two frames (delta time). Computers have got different operation times.
    const float timeStep = 1.0f / 60.0f; // 1 secund
    float accumulator = 0.0f;
    int previousTick = 0;

    float cameraY; // camera

    // platforms
    std::list<Platform*> platforms;

    Sprite* platformSprite;
    int platformCounter = 0;
    int platformMargin = 70;
    const int platformSpeed = 4;

    // enemies
    std::list<Enemy*> enemies;
    Sprite* enemySprite;
    int enemySpawnInterval = 15;

    // ability
    std::list<Ability*> abilities;
    Sprite* abilitySprite;
    int abilitySpawnInterval = 25;

    // projectiles
    Projectile* projectile;
    Sprite* projectileSprite;
    const int projectileSpeed = 8;

    // mouse
    int mouseX, mouseY;

    // score
    int score = 0;
    float platformsMoved = 0.0f;
    int jumpCount = 0;
};