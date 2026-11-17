#include "WinScene.h"

WinScene::WinScene()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

WinScene::WinScene(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

WinScene::~WinScene() { shutdown(); }

void WinScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.isPlayerHit = false;

    mGameState.diver = nullptr;
    mGameState.map  = nullptr;
    mGameState.enemies = nullptr;
    mGameState.enemyCount = 0;

    mMessageTexture = LoadTexture("assets/game/win.png");
    mPromptTexture = LoadTexture("assets/game/menu.png");
}

void WinScene::update(float deltaTime) {}

void WinScene::render()
{
    renderImage(mMessageTexture, 700.0f, {mOrigin.x, mOrigin.y - 80.0f});
    renderImage(mPromptTexture, 600.0f, {mOrigin.x, mOrigin.y + 120.0f});
}

void WinScene::shutdown()
{
    UnloadTexture(mMessageTexture);
    UnloadTexture(mPromptTexture);

    mMessageTexture = {};
    mPromptTexture = {};
}
