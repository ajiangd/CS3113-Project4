#include "LevelB.h"

LevelB::LevelB()
    : Scene{{0.0f, 0.0f}, "#000000"}
{}

LevelB::LevelB(Vector2 origin, const char *bgHexCode)
    : Scene{origin, bgHexCode}
{}

LevelB::~LevelB() {shutdown();}

void LevelB::initialise()
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

    createCrab(&mGameState.enemies[0], getTilePosition(18, 7), 70);
    createCrab(&mGameState.enemies[1], getTilePosition(40, 7), 70);
    createShark(&mGameState.enemies[2], getTilePosition(25, 2), 130);
    createShark(&mGameState.enemies[3], getTilePosition(44, 2), 120);
    createJellyfish(&mGameState.enemies[4], getTilePosition(43, 2), 0.6f);
}

void LevelB::update(float deltaTime)
{
    updateLevel(deltaTime, LEVEL_C);
}

void LevelB::render()
{
    renderLevel();
}

void LevelB::shutdown()
{
    shutdownLevel();
}
