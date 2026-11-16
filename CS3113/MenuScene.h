#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
private:
    Texture2D mTitleTexture = {};
    Texture2D mPromptTexture = {};
    Texture2D mInstructionsTexture = {};

public:
    MenuScene();
    MenuScene(Vector2 origin, const char *bgHexCode);
    ~MenuScene();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
