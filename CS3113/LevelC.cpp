#include "LevelC.h"

LevelC::LevelC()
    : Scene{{0.0f, 0.0f}, "#000000"}
{}

LevelC::LevelC(Vector2 origin, const char *bgHexCode)
    : Scene{origin, bgHexCode}
{}

LevelC::~LevelC() {shutdown();}

void LevelC::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.isPlayerHit = false;

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        mLevelData,
        "assets/game/tiles.png",
        TILE_DIMENSION,
        11, 12,
        {LEVEL_WIDTH * TILE_DIMENSION / 2.0f, mOrigin.y}
    );

    createDiver(getTilePosition(2, 7));

    mGameState.enemyCount = ENEMY_COUNT;
    mGameState.enemies = new Entity[ENEMY_COUNT];

    createCrab(&mGameState.enemies[0], getTilePosition(14, 7), 90);
    createCrab(&mGameState.enemies[1], getTilePosition(33, 7), 100);
    createCrab(&mGameState.enemies[2], getTilePosition(53, 7), 110);
    createShark(&mGameState.enemies[3], getTilePosition(30, 2), 150);
    createShark(&mGameState.enemies[4], getTilePosition(55, 2), 160);
    createJellyfish(&mGameState.enemies[5], getTilePosition(41, 1), 0.7f);
    createJellyfish(&mGameState.enemies[6], getTilePosition(57, 2), 0.8f);
}

void LevelC::update(float deltaTime)
{
    updateLevel(deltaTime, WIN_SCENE);
}

void LevelC::render()
{
    renderLevel();
}

void LevelC::shutdown()
{
    shutdownLevel();
}
