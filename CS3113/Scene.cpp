#include "Scene.h"

Scene::Scene() : mGameState{}, mOrigin{} {}

Scene::Scene(Vector2 origin, const char *bgHexCode)
    : mGameState{}, mOrigin{ origin }, mBGColourHexCode{ bgHexCode }
{}

void Scene::renderImage(Texture2D texture, float width, Vector2 centre)
{
    float height = width * texture.height / texture.width;

    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)
    };

    Rectangle destinationArea = { centre.x, centre.y, width, height };

    Vector2 originOffset = { width / 2.0f, height / 2.0f };

    DrawTexturePro(
        texture, textureArea, destinationArea, originOffset, 0.0f, WHITE
    );
}

Vector2 Scene::getTilePosition(int col, int row)
{
    return {
        mGameState.map->getLeftBoundary() + col * TILE_DIMENSION + TILE_DIMENSION / 2.0f,
        mGameState.map->getTopBoundary() + row * TILE_DIMENSION + TILE_DIMENSION / 2.0f
    };
}

void Scene::createDiver(Vector2 position)
{
    std::map<Direction, std::vector<int>> diverAnimationAtlas = {
        {RIGHT, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
        {LEFT, {10, 11, 12, 13, 14, 15, 16, 17, 18, 19}}
    };

    mGameState.diver = new Entity(
        position,
        {120.0f, 47.0f},
        "assets/game/diver.png",
        ATLAS,
        {2, 10},
        diverAnimationAtlas,
        PLAYER
    );

    mGameState.diver->setColliderDimensions({96.0f, 35.0f});
    mGameState.diver->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.diver->setJumpingPower(450.0f);
    mGameState.diver->setSpeed(220);
}

void Scene::createCrab(Entity *crab, Vector2 position, int speed)
{
    std::map<Direction, std::vector<int>> crabAnimationAtlas = {
        {RIGHT, {0, 1}},
        {LEFT,  {0, 1}}
    };

    crab->setTexture("assets/game/crab.png");
    crab->setTextureType(ATLAS);
    crab->setSpriteSheetDimensions({1, 2});
    crab->setAnimationAtlas(crabAnimationAtlas);
    crab->setFrameSpeed(6);
    crab->setScale({64.0f, 64.0f});
    crab->setColliderDimensions({52.0f, 52.0f});
    crab->setPosition(position);
    crab->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    crab->setSpeed(speed);
    crab->setEntityType(NPC);
    crab->setAIType(WANDERER);
    crab->setAIState(WALKING);
}

void Scene::createShark(Entity *shark, Vector2 position, int speed)
{
    std::map<Direction, std::vector<int>> sharkAnimationAtlas = {
        {RIGHT, {0}},
        {LEFT, {1}}
    };

    shark->setTexture("assets/game/shark.png");
    shark->setTextureType(ATLAS);
    shark->setSpriteSheetDimensions({1, 2});
    shark->setAnimationAtlas(sharkAnimationAtlas);
    shark->setFrameSpeed(1);
    shark->setScale({120.0f, 30.0f});
    shark->setColliderDimensions({100.0f, 24.0f});
    shark->setPosition(position);
    shark->setSpeed(speed);
    shark->setEntityType(NPC);
    shark->setAIType(FOLLOWER);
    shark->setAIState(IDLE);
}

void Scene::createJellyfish(Entity *jellyfish, Vector2 position, float lerpFactor)
{
    std::map<Direction, std::vector<int>> jellyfishAnimationAtlas = {
        {RIGHT, {0, 1, 2, 3}},
        {LEFT,  {0, 1, 2, 3}}
    };

    jellyfish->setTexture("assets/game/jellyfish_large.png");
    jellyfish->setTextureType(ATLAS);
    jellyfish->setSpriteSheetDimensions({1,4});
    jellyfish->setAnimationAtlas(jellyfishAnimationAtlas);
    jellyfish->setFrameSpeed(6);
    jellyfish->setScale({72.0f, 72.0f});
    jellyfish->setColliderDimensions({54.0f, 48.0f});
    jellyfish->setPosition(position);
    jellyfish->setEntityType(NPC);
    jellyfish->setAIType(LERPER);
    jellyfish->setAIState(IDLE);
    jellyfish->setLerpFactor(lerpFactor);
}

void Scene::updateLevel(float deltaTime, SceneID nextLevel)
{
    mGameState.diver->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

    for (int i = 0; i < mGameState.enemyCount; i++)
    {
        mGameState.enemies[i].update(
            deltaTime, mGameState.diver, mGameState.map, nullptr, 0
        );

        if (mGameState.diver->isColliding(&mGameState.enemies[i]))
            mGameState.isPlayerHit = true;
    }

    if (mGameState.diver->getPosition().y > END_GAME_THRESHOLD)
        mGameState.isPlayerHit = true;

    if (mGameState.diver->getPosition().x >
        mGameState.map->getRightBoundary() - 3 * TILE_DIMENSION)
        mGameState.nextSceneID = nextLevel;
}

void Scene::renderLevel()
{
    mGameState.map->render();

    for (int i = 0; i < mGameState.enemyCount; i++)
        mGameState.enemies[i].render();

    mGameState.diver->render();
}

void Scene::shutdownLevel()
{
    delete mGameState.diver;
    delete mGameState.map;
    delete[] mGameState.enemies;

    mGameState.diver = nullptr;
    mGameState.map  = nullptr;
    mGameState.enemies  = nullptr;
    mGameState.enemyCount = 0;
}