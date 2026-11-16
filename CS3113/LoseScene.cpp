#include "LoseScene.h"

LoseScene::LoseScene()
    : Scene{{0.0f, 0.0f}, "#000000"}
{}

LoseScene::LoseScene(Vector2 origin, const char *bgHexCode)
    : Scene{origin, bgHexCode}
{}

LoseScene::~LoseScene() {shutdown();}

void LoseScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.isPlayerHit = false;

    mGameState.diver = nullptr;
    mGameState.map = nullptr;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;

    mMessageTexture = LoadTexture("assets/game/lose.png");
    mPromptTexture = LoadTexture("assets/game/menu.png");
}

void LoseScene::update(float deltaTime) {}

void LoseScene::render()
{
    renderImage(mMessageTexture, 700.0f, {mOrigin.x, mOrigin.y - 80.0f});
    renderImage(mPromptTexture, 600.0f, {mOrigin.x, mOrigin.y + 120.0f});
}

void LoseScene::shutdown()
{
    UnloadTexture(mMessageTexture);
    UnloadTexture(mPromptTexture);

    mMessageTexture = {};
    mPromptTexture = {};
}
