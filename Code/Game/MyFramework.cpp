#include "MyFramework.h"


void MyFramework::PreInit(int& width, int& height, bool& fullScreen)
{
    fullScreen = false;

    if (!fullScreen)
    {
        for (int i = 1; i < __argc; i++) {
            if (strcmp(__argv[i], "-window") == 0 && i + 2 < __argc) {
                width = strtoul(__argv[++i], NULL, 10);
                height = strtoul(__argv[++i], NULL, 10);
            }

        }
        std::cout << "\t\t\t\t\t\tWidth Window - " << width << "\n\t\t\t\t\t\tHeight Window - " << height << std::endl;
    }

    this->width = width;
    this->height = height;
}

bool MyFramework::Init()
{
    playerSprite = createSprite("data\\dragon.png");
    if (!playerSprite)
    {
        throw std::runtime_error(" During loading the picture");
    }
    dragon = new Dragon(playerSprite, width / 2, height, -400);

    enemySprite = createSprite("data\\enemy.png");

    if (!enemySprite)
    {
        throw std::runtime_error(" During loading the picture");
    }

    abilitySprite = createSprite("data\\immunity.png");
    if (!abilitySprite)
    {
        throw std::runtime_error(" During loading the picture");
    }

    platformSprite = createSprite("data\\platform.png");
    if (!platformSprite)
    {
        throw std::runtime_error(" During loading the picture");
    }

    projectileSprite = createSprite("data\\projectile.png");
    if (!projectileSprite)
    {
        throw std::runtime_error(" During loading the picture");
    }
    projectile = new Projectile(projectileSprite, -50000, -50000, projectileSpeed);
    previousTick = getTickCount();

    firstGenPlatforms();

    return true;
}

void MyFramework::Close()
{
    destroySprite(dragon->sprite);
    delete dragon;


    for (auto enemy : enemies)
    {
        delete& enemy->sprite;
        enemy = nullptr;
        delete enemy;
    }
    enemies.clear();

    for (auto platform : platforms)
    {
        delete& platform->sprite;
        platform = nullptr;
        delete platform;
    }
    platforms.clear();

    for (auto ability : abilities)
    {
        delete& ability->sprite;
        ability = nullptr;
        delete ability;
    }
    abilities.clear();


    destroySprite(projectileSprite);
    delete projectile;
}

bool MyFramework::Tick() {

    // DeltaTime
    int currentTick = getTickCount();
    float DeltaTime = (currentTick - previousTick) / 1000.0f;
    previousTick = currentTick;

    accumulator += DeltaTime;

    while (accumulator >= timeStep)
    {
        dragon->setY(dragon->getY() + (dragon->jumpSpeed * timeStep));
        dragon->jumpSpeed += gravity * timeStep;
        accumulator -= timeStep;
    }

    dragon->setX(dragon->getX() + dragon->moveSpeed);
    dragon->jumpSpeed += gravity;
    // ---------------------------

    if (dragon->getX() < 0)
        dragon->setX(width - dragon->width);
    else if (dragon->getX() > width - dragon->width)
        dragon->setX(0);

    if (projectile->x < 0)
        projectile->x = width - projectile->width;
    else if (projectile->x > width - projectile->width)
        projectile->x = 0;


    if (dragon->getY() > height)
    {
        checkHealth();
    }


    if (currentTick >= momentPickedImmunity + 20000)
    {
        if (momentPickedImmunity != 0)
        {
            std::cout << "\nImmunity is over!";
        }
        momentPickedImmunity = 0;
        dragon->isPickedImmunity = false;
    }

    updateCamera();

    showCursor(true);

    handleCollisions();

    handleProjectiles();

    drawTestBackground();

    drawSprite(dragon->sprite, dragon->getX(), dragon->getY());

    for (auto& platform : platforms)
    {
        platform->draw(cameraY);
    }

    for (auto& enemy : enemies)
    {
        enemy->draw(cameraY);
    }

    for (auto& ability : abilities)
    {
        ability->draw(cameraY);
    }

    projectile->draw(cameraY);


    int index = 0; 
    for (Platform* platform : platforms)
    {
        if (index % 6 == 0)
        {
            platform->canHaveSomething = false;

            if (platform->isMoving)
            {
                platform->x += 1;
            }
            else
            {
                platform->x -= 1;
            }

            if (platform->x <= 0 || platform->x >= width - platform->width)
            {
                platform->isMoving = !platform->isMoving;
            }
        }
        index++;
    }

    return false;
}

void MyFramework::onMouseMove(int x, int y, int xRelative, int yRelative)
{
    mouseX = x;
    mouseY = y;
}

void MyFramework::onMouseButtonClick(FRMouseButton button, bool isReleased)
{
    if (button == FRMouseButton::LEFT && !isReleased)
    {
        if (!projectile->active)
        {
            int dx = mouseX - (dragon->getX() + dragon->width / 2);
            int dy = mouseY - (dragon->getY() + dragon->height / 2);
            float directionMagnitude = sqrt(dx * dx + dy * dy);

            if (directionMagnitude != 0)
            {
                float angle = atan2(dy, dx) * (180.0f / M_PI);

                if (angle < 0)
                {
                    angle += 360;
                }

                if (angle >= 90 && angle < 150)
                {
                    angle = 360 - angle;
                }
                else if (angle >= 150 && angle < 210)
                {
                    angle = 210;
                }
                else if ((angle >= 330 && angle < 360) || (angle >= 0 && angle < 30))
                {
                    angle = 330;
                }
                else if (angle >= 30 && angle < 90)
                {
                    angle = 360 - angle;
                }

                angle = angle * M_PI / 180.0f;

                projectile->directionX = cos(angle) * projectileSpeed;
                projectile->directionY = sin(angle) * projectileSpeed;

                projectile->x = dragon->getX() + (dragon->width - projectile->width) / 2;
                projectile->y = dragon->getY();
                projectile->active = true;
            }
        }
    }
}

void MyFramework::onKeyPressed(FRKey k)
{
    switch (k)
    {
    case FRKey::LEFT:
        dragon->move(-3);
        break;
    case FRKey::RIGHT:
        dragon->move(3);
        break;
    default:
        break;
    }
}

void MyFramework::onKeyReleased(FRKey k)
{
    switch (k)
    {
    case FRKey::LEFT:
    case FRKey::RIGHT:
        dragon->stop();
        break;
    default:
        break;
    }
}

const char* MyFramework::GetTitle()
{
    return "Dragon Jump";
}

void MyFramework::updateCamera()
{
    if (dragon->getY() <= height / 2)
    {
        jumpCount++;

        float deltaY = height / 2 - dragon->getY();
        dragon->setY(height / 2);

        const float platformMarginX = 100.0f;
        const float platformMarginY = 20.0f;

        for (auto it = platforms.begin(); it != platforms.end(); )
        {
            Platform* platform = *it;
            platform->y += deltaY;

            if (platform->y > height)
            {
                platform->y -= height;

                platformsMoved += abs(deltaY);
                score = static_cast<int>(platformsMoved) * 8;

                int maxAttempts = 10;
                bool foundNonColliding = false;

                for (int attempt = 0; attempt < maxAttempts; ++attempt)
                {
                    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * width;

                    bool collides = any_of(platforms.begin(), platforms.end(), [&](Platform* p)
                        {
                            return abs(p->x - x) < platformMarginX && abs(p->y - platform->y) < platformMarginY;
                        });
                    if (!collides)
                    {
                        platform->x = x;
                        foundNonColliding = true;
                        break;
                    }
                }

                if (foundNonColliding)
                {
                    ++it;
                }
                else
                {
                    delete platform;
                    it = platforms.erase(it);
                }

                if (platformCounter % abilitySpawnInterval == 0)
                {
                    if (platform->canHaveSomething == true)
                    {
                        Sprite* newAbilitySprite = createSprite("data\\immunity.png");
                        Ability* ability = new Ability(newAbilitySprite, platform->x + 15, platform->y - 43);
                        abilities.push_back(ability);
                    }
                }

                if (platformCounter % enemySpawnInterval == 0)
                {
                    if (platform->canHaveSomething == true)
                    {
                        std::cout << "\nBe careful MONSTER !";
                        Sprite* newEnemySprite = createSprite("data\\enemy.png");
                        Enemy* enemy = new Enemy(newEnemySprite, platform->x + 15, platform->y - 43);
                        enemies.push_back(enemy);
                    }
                }

                platformCounter++;
            }
            else
            {
                ++it;
            }
        }
        for (auto it = enemies.begin(); it != enemies.end(); ++it)
        {
            Enemy* enemy = *it;
            enemy->y += deltaY;
        }
        for (auto it = abilities.begin(); it != abilities.end(); ++it)
        {
            Ability* ability = *it;
            ability->getY() += deltaY;
        }
    }
}

void MyFramework::firstGenPlatforms()
{
    int highest_platform_y = height;

    for (int i = 0; i < 3333; ++i)
    {
        float randomXDistance = 6.0f * width / 35.0f + static_cast<float>(std::rand()) / RAND_MAX * (13.0f * width / 15.0f - 6.0f * width / 35.0f);

        int randomYDistance = std::rand() % (height - platformMargin + 1);

        int x = randomXDistance;
        int y = randomYDistance;

        bool collides = any_of(platforms.begin(), platforms.end(), [&](Platform* p)
            {
                return abs(p->x - x) < 220 && abs(p->y - y) < 50;
            });

        if (!collides)
        {

            platforms.push_back(new Platform(platformSprite, x, y));
            highest_platform_y = std::min(highest_platform_y, y);

            platformCounter++;
        }
    }
}

void MyFramework::handleCollisions()
{

    for (auto& platform : platforms)
    {
        int platformX, platformY, platformWidth, platformHeight;
        platformX = platform->x;
        platformY = platform->y - cameraY;
        platformWidth = platform->width / 2;
        platformHeight = platform->height / 3;

        if (dragon->getX() < platformX + platformWidth &&
            dragon->getX() + dragon->width / 2 > platformX &&
            dragon->getY() + dragon->height > platformY &&
            dragon->getY() + dragon->height < platformY + platformHeight &&
            dragon->jumpSpeed > 0)
        {
            dragon->jumpSpeed = -400;
        }
    }

    auto enemy_it = enemies.begin();
    while (enemy_it != enemies.end())
    {
        Enemy* enemy = *enemy_it;
        if (dragon->getX() < enemy->x + enemy->width &&
            dragon->getX() + dragon->width > enemy->x &&
            dragon->getY() + dragon->height > enemy->y &&
            dragon->getY() < enemy->y + enemy->height)
        {

            if (dragon->jumpSpeed > 0 && dragon->getY() <= enemy->y - 20)
            {
                destroySprite(enemy->sprite);
                delete enemy;
                enemy_it = enemies.erase(enemy_it);
                dragon->jumpSpeed = -400;
                continue;
            }
            else if (dragon->isPickedImmunity == false) {
                dragon->setX(height + 1);
                if (!checkHealth()) { return; }
            }
        }
        ++enemy_it;
    }

    auto ability_it = abilities.begin();
    while (ability_it != abilities.end())
    {
        Ability* ability = *ability_it;

        if (dragon->getX() < ability->getX() + ability->getWidth() &&
            dragon->getX() + dragon->width > ability->getX() &&
            dragon->getY() + dragon->height > ability->getY() &&
            dragon->getY() < ability->getY() + ability->getHeight())
        {
            destroySprite(ability->sprite);
            delete ability;
            ability_it = abilities.erase(ability_it);

            dragon->isPickedImmunity = true;
            momentPickedImmunity = getTickCount();
            std::cout << "\nYou have picked up immunity for 20 seconds !" << std::endl;
            continue;
        }
        ++ability_it;
    }
}

void MyFramework::handleProjectiles()
{
    projectile->update(cameraY, height);

    auto enemy_it = enemies.begin();
    while (enemy_it != enemies.end())
    {
        Enemy* enemy = *enemy_it;
        if (projectile->active &&
            projectile->x < enemy->x + enemy->width &&
            projectile->x + projectile->width > enemy->x &&
            projectile->y < enemy->y + enemy->height &&
            projectile->y + projectile->height > enemy->y)
        {

            destroySprite(enemy->sprite);
            delete enemy;
            enemy_it = enemies.erase(enemy_it);
            projectile->active = false;

            projectile->x = -50000;
            projectile->y = -50000;

            continue;
        }

        ++enemy_it;
    }
}

bool MyFramework::checkHealth()
{
    if (numberLives == 1)
    {
        std::cout << "Game Over\n Your score was: " << score << "\nYou passed " << platformsMoved << " platforms!\n";
        platforms.clear();
        firstGenPlatforms();
        enemies.clear();
        platformsMoved = 0.0f;
        score = 0;
        numberLives = 5;
        return false;
    }
    else
    {
        dragon->setX((width - dragon->width) / 2);
        dragon->setY(height - dragon->height);
        dragon->jumpSpeed = -500;
        cameraY = 0;
        numberLives--;
        std::cout << "\tYour LIVES - " << numberLives << std::endl;
    }
}

void MyFramework::HandleExeption(const std::exception& exe)
{
    system("cls");

    std::cout << "Error!";
    std::cout << exe.what() << "\n";

    exit(0);
}
