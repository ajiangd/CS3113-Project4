#ifndef WINSCENE_H
#define WINSCENE_H

#include "Scene.h"

class WinScene : public Scene
{
private:
    Texture2D mMessageTexture = {};
    Texture2D mPromptTexture = {};

public:
    WinScene();
    WinScene(Vector2 origin, const char *bgHexCode);
    ~WinScene();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
