#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct GameState
{
    Entity *diver;
    Map    *map;

    Entity *enemies;
    int     enemyCount;

    bool isPlayerHit;

    SceneID nextSceneID;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

    void renderImage(Texture2D texture, float width, Vector2 centre);

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() {}

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;       }
    Vector2     getOrigin()          const { return mOrigin;          }
    const char *getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif
