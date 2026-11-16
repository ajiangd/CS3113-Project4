#include "LevelA.h"

LevelA::LevelA()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelA::LevelA(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.isPlayerHit = false;

    mGameState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        mLevelData,
        "assets/game/tiles.png",
        TILE_DIMENSION,
        11, 12,
        { LEVEL_WIDTH * TILE_DIMENSION / 2.0f, mOrigin.y }
    );

    createDiver(getTilePosition(2, 7));

    mGameState.enemyCount = ENEMY_COUNT;
    mGameState.enemies    = new Entity[ENEMY_COUNT];

    createCrab(&mGameState.enemies[0], getTilePosition(16, 7), 80);
    createShark(&mGameState.enemies[1], getTilePosition(29, 4), 100);
    createJellyfish(&mGameState.enemies[2], getTilePosition(37, 2), 0.5f);
}

void LevelA::update(float deltaTime)
{
    updateLevel(deltaTime, LEVEL_B);
}

void LevelA::render()
{
    renderLevel();
}

void LevelA::shutdown()
{
    shutdownLevel();
}
