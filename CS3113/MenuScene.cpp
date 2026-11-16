#include "MenuScene.h"

MenuScene::MenuScene()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

MenuScene::MenuScene(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.isPlayerHit = false;

    mGameState.diver      = nullptr;
    mGameState.map        = nullptr;
    mGameState.enemies    = nullptr;
    mGameState.enemyCount = 0;

    mTitleTexture = LoadTexture("assets/game/title.png");
    mPromptTexture = LoadTexture("assets/game/start.png");
}

void MenuScene::update(float deltaTime) {}

void MenuScene::render()
{
    renderImage(mTitleTexture, 800.0f, { mOrigin.x, mOrigin.y - 80.0f });
    renderImage(mPromptTexture, 600.0f, { mOrigin.x, mOrigin.y + 120.0f });
}

void MenuScene::shutdown()
{
    UnloadTexture(mTitleTexture);
    UnloadTexture(mPromptTexture);

    mTitleTexture = {};
    mPromptTexture = {};
}
