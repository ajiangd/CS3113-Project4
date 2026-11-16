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
