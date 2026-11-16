#include "CS3113/MenuScene.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/WinScene.h"
#include "CS3113/LoseScene.h"

constexpr int SCREEN_WIDTH = 1000,
              SCREEN_HEIGHT = 600,
              FPS = 120,
              MAX_LIVES = 3,
              HEART_FULL = 0,
              HEART_EMPTY = 12;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f,
                HEART_SIZE = 40.0f;

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f,
      gTimeAccumulator = 0.0f;

Camera2D gCamera = {};

Scene *gCurrentScene = nullptr;
SceneID gCurrentSceneID = NO_SCENE;
std::map<SceneID, Scene *> gLevels = {};

int gLives = MAX_LIVES;

Texture2D gBackgroundTexture;
Texture2D gHeartsTexture;

Music gBackgroundMusic;
Sound gJumpSound;
Sound gSplashSound;
Sound gWinSound;

void switchToScene(SceneID id);
void initialise();
void processInput();
void update();
void renderLives();
void render();
void shutdown();

void switchToScene(SceneID id)
{
    if (gCurrentScene) gCurrentScene->shutdown();

    gCurrentScene = gLevels[id];
    gCurrentSceneID = id;
    gCurrentScene->initialise();

    if (id == MENU_SCENE) gLives = MAX_LIVES;
    if (id == WIN_SCENE) PlaySound(gWinSound);

    if (gCurrentScene->getState().diver != nullptr)
        gCamera.target = {
            gCurrentScene->getState().diver->getPosition().x, ORIGIN.y
        };
    else
        gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Deep Dive");
    InitAudioDevice();

    gBackgroundTexture = LoadTexture("assets/game/background.png");
    gHeartsTexture = LoadTexture("assets/game/hearts.png");

    gBackgroundMusic = LoadMusicStream("assets/game/bgm.ogg");
    SetMusicVolume(gBackgroundMusic, 0.8f);
    PlayMusicStream(gBackgroundMusic);

    gJumpSound = LoadSound("assets/game/bubble.wav");
    gSplashSound = LoadSound("assets/game/splash.ogg");
    gWinSound = LoadSound("assets/game/win.wav");

    gLevels[MENU_SCENE] = new MenuScene(ORIGIN, "#0B3D6B");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#0B3D6B");
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#0B3D6B");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#0B3D6B");
    gLevels[WIN_SCENE] = new WinScene(ORIGIN, "#0B3D6B");
    gLevels[LOSE_SCENE] = new LoseScene(ORIGIN, "#0B3D6B");

    gCamera.offset = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom = 1.0f;

    switchToScene(MENU_SCENE);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (IsKeyPressed(KEY_ONE)) switchToScene(LEVEL_A);
    else if (IsKeyPressed(KEY_TWO)) switchToScene(LEVEL_B);
    else if (IsKeyPressed(KEY_THREE)) switchToScene(LEVEL_C);

    if (IsKeyPressed(KEY_ENTER))
    {
        if (gCurrentSceneID == MENU_SCENE)
            switchToScene(LEVEL_A);
        else if (gCurrentSceneID == WIN_SCENE || gCurrentSceneID == LOSE_SCENE)
            switchToScene(MENU_SCENE);
    }

    Entity *diver = gCurrentScene->getState().diver;

    if (diver == nullptr) return;

    diver->resetMovement();

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) diver->moveLeft();
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) diver->moveRight();

    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) &&
        diver->isCollidingBottom())
    {
        diver->jump();
        PlaySound(gJumpSound);
    }
}

void update()
{
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        UpdateMusicStream(gBackgroundMusic);

        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        if (gCurrentScene->getState().diver != nullptr)
        {
            Vector2 currentPlayerPosition = {
                gCurrentScene->getState().diver->getPosition().x, ORIGIN.y
            };

            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void renderLives()
{
    for (int i = 0; i < MAX_LIVES; i++)
    {
        int heartIndex = i < gLives ? HEART_FULL : HEART_EMPTY;

        Rectangle textureArea = getUVRectangle(&gHeartsTexture, heartIndex, 1, 13);

        Rectangle destinationArea = {
            20.0f + i * (HEART_SIZE + 10.0f), 20.0f, HEART_SIZE, HEART_SIZE
        };

        DrawTexturePro(
            gHeartsTexture, textureArea, destinationArea, {0.0f, 0.0f}, 0.0f, WHITE
        );
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(gCurrentScene->getBGColourHexCode()));

    DrawTexturePro(
        gBackgroundTexture,
        {
            0.0f, 0.0f,
            static_cast<float>(gBackgroundTexture.width),
            static_cast<float>(gBackgroundTexture.height)
        },
        {0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT},
        {0.0f, 0.0f}, 0.0f, WHITE
    );

    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentScene->getState().diver != nullptr) renderLives();

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;

    gLevels.clear();

    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gHeartsTexture);

    UnloadMusicStream(gBackgroundMusic);
    UnloadSound(gJumpSound);
    UnloadSound(gSplashSound);
    UnloadSound(gWinSound);

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().isPlayerHit)
        {
            gLives--;
            PlaySound(gSplashSound);

            if (gLives <= 0) switchToScene(LOSE_SCENE);
            else switchToScene(gCurrentSceneID);
        }
        else if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            switchToScene(gCurrentScene->getState().nextSceneID);
        }

        render();
    }

    shutdown();

    return 0;
}
