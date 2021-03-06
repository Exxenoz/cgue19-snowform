#include "GameScene.h"
#include "UserInterfaceScene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "ImageCutOffMaterial.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "CameraComponent.h"
#include "Time.h"
#include <GLFW/glfw3.h>

GameScene::GameScene() :
    Scene::Scene("Game Scene"),
    gameState(GAME_STATE_INGAME_PLAYING),
    won(false),
    nextLevelScene(""),
    coinCount(0),
    collectedCoinCount(0),
    remainingTimeInSecondsOption(0.f),
    remainingTimeInSeconds(0.f),
    transitionTimeInSecondsOption(0.f),
    transitionTimeInSeconds(0.f),
    debugContainerEnabled(false),
    debugContainerKeyPressed(false),
    toggleViewFrustumKeyPressed(false),
    autoStartNextLevel(true)
{
}

GameScene::~GameScene()
{
}

void GameScene::OnLoad()
{
    won = false;
    remainingTimeInSeconds = remainingTimeInSecondsOption;
}

void GameScene::OnUpdate()
{
    switch (gameState)
    {
        case GAME_STATE_INGAME_PLAYING:
        {
            UpdateRemainingTime();
            UpdateDebugCommands();
            break;
        }
        case GAME_STATE_OVERLAY_TRANSITION:
        {
            UpdateOverlayTransition();
            break;
        }
        case GAME_STATE_OVERLAY_PAUSE:
        {
            if (autoStartNextLevel || sInputManager.IsAnyKeyPressed())
            {
                EnableOverlay(false);

                if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
                {
                    userInterfaceScene->DisableOverlayTexts();
                }
            }
            break;
        }
        case GAME_STATE_CREDITS:
        {
            UpdateCredits();
            break;
        }
    }
}

void GameScene::UpdateRemainingTime()
{
    if (remainingTimeInSeconds > 0.f)
    {
        remainingTimeInSeconds -= sTime.GetDeltaTime();

        if (remainingTimeInSeconds <= 0.f)
        {
            remainingTimeInSeconds = 0.f;

            if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
            {
                userInterfaceScene->EnableTimeIsUpText(true);
            }

            RestartLevel();
        }

        if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            int minutesLeft = ((int)remainingTimeInSeconds) / 60;
            int secondsLeft = ((int)remainingTimeInSeconds) % 60;

            std::string minutesLeftString = minutesLeft < 10 ? "0" + std::to_string(minutesLeft) : std::to_string(minutesLeft);
            std::string secondsLeftString = secondsLeft < 10 ? "0" + std::to_string(secondsLeft) : std::to_string(secondsLeft);
            std::string timeLeft = minutesLeftString + ":" + secondsLeftString;
            userInterfaceScene->SetRemainingTimeText(timeLeft);
        }
    }
}

void GameScene::UpdateOverlayTransition()
{
    if (transitionTimeInSeconds > 0.f)
    {
        transitionTimeInSeconds -= sTime.GetUnscaledDeltaTime();

        if (transitionTimeInSeconds <= 0.f)
        {
            transitionTimeInSeconds = 0.f;

            SetCoinCount(0);
            SetCollectedCoinCount(0);

            if (won) // Load next level
            {
                Unload();

                if (!nextLevelScene.empty())
                {
                    LoadFromFile(nextLevelScene);

                    gameState = GAME_STATE_OVERLAY_PAUSE;
                }
                else if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
                {
                    userInterfaceScene->Unload();
                    userInterfaceScene->LoadFromFile("assets/scenes/credits_scene.xml");

                    gameState = GAME_STATE_CREDITS;
                }
            }
            else // Restart level
            {
                Reload();

                gameState = GAME_STATE_OVERLAY_PAUSE;
            }
        }

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetTransitionOverlayCutOff(transitionTimeInSeconds / transitionTimeInSecondsOption);
        }
    }
    else if (transitionTimeInSeconds < 0.f)
    {
        transitionTimeInSeconds += sTime.GetUnscaledDeltaTime();

        if (transitionTimeInSeconds >= 0.f)
        {
            transitionTimeInSeconds = 0.f;

            if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
            {
                userInterfaceScene->EnableTransitionOverlay(false);
            }

            gameState = GAME_STATE_INGAME_PLAYING;

            sTime.Resume();
        }

        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetTransitionOverlayCutOff((transitionTimeInSecondsOption + transitionTimeInSeconds) / transitionTimeInSecondsOption);
        }
    }
}

void GameScene::SetOption(std::string const& key, std::string const& value)
{
    if (key == "NextLevelScene")
    {
        nextLevelScene = value;
    }
    else if (key == "RemainingTime")
    {
        if (value.size() != 5)
        {
            fprintf(stderr, "Could not set game scene option '%s', because the value '%s' is invalid!\n", key.c_str(), value.c_str());
            return;
        }

        std::string minutes = value.substr(0, 2);
        std::string seconds = value.substr(3, 2);

        remainingTimeInSecondsOption = (float)(std::stoi(minutes) * 60 + std::stoi(seconds));
    }
    else if (key == "TransitionTime")
    {
        transitionTimeInSecondsOption = (float)std::atof(value.c_str());
    }
    else if (key == "LevelText")
    {
        if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->SetLevelText(value);
        }
    }
    else
    {
        fprintf(stderr, "Could not set unknown game scene option '%s'!\n", key.c_str());
        return;
    }

    fprintf(stdout, "Set game scene option '%s' with value '%s'!\n", key.c_str(), value.c_str());
}

void GameScene::SetCoinCount(int coinCount)
{
    this->coinCount = coinCount;

    if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        std::string coinCountString = std::to_string(collectedCoinCount) + "/" + std::to_string(coinCount);
        userInterfaceScene->SetCoinCountText(coinCountString);
    }
}

void GameScene::SetCollectedCoinCount(int collectedCoinCount)
{
    this->collectedCoinCount = collectedCoinCount;

    if (UserInterfaceScene* userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        std::string coinCountString = std::to_string(collectedCoinCount) + "/" + std::to_string(coinCount);
        userInterfaceScene->SetCoinCountText(coinCountString);
    }
}

void GameScene::CompleteLevel()
{
    if (gameState != GAME_STATE_INGAME_PLAYING)
    {
        return;
    }

    fprintf(stdout, "Complete Level!\n");

    won = true;

    sTime.Pause();

    if (nextLevelScene.empty())
    {
        // Disable overlay texts, because next scene is credits scene
        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            userInterfaceScene->DisableOverlayTexts();
        }
    }

    EnableOverlay(true);
}

void GameScene::RestartLevel()
{
    if (gameState != GAME_STATE_INGAME_PLAYING)
    {
        return;
    }

    fprintf(stdout, "Restart level!\n");

    sTime.Pause();

    EnableOverlay(true);
}

void GameScene::EnableOverlay(bool enable)
{
    gameState = GAME_STATE_OVERLAY_TRANSITION;

    if (ImageCutOffMaterial* im = sAssetManager.GetMaterial<ImageCutOffMaterial>("TransitionOverlay"))
    {
        switch (rand() % 3)
        {
            case 0:
                im->SetCutOffTexture(sAssetManager.GetTexture2D("transition_overlay_cutoff"));
                break;
            case 1:
                im->SetCutOffTexture(sAssetManager.GetTexture2D("transition_overlay_cutoff2"));
                break;
            case 2:
                im->SetCutOffTexture(sAssetManager.GetTexture2D("transition_overlay_cutoff3"));
                break;
            default:
                im->SetCutOffTexture(sAssetManager.GetTexture2D("transition_overlay_cutoff"));
                break;
        }
    }

    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->EnableTransitionOverlay(true);
        userInterfaceScene->SetTransitionOverlayCutOff(enable ? 1.f : 0.f);
    }

    transitionTimeInSeconds = enable ? transitionTimeInSecondsOption : -transitionTimeInSecondsOption;
}

void GameScene::UpdateDebugCommands()
{
    if (sInputManager.IsKeyPressed(GLFW_KEY_F10))
    {
        CompleteLevel();
    }
    else if (sInputManager.IsKeyPressed(GLFW_KEY_F11))
    {
        RestartLevel();
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_F8))
    {
        if (!toggleViewFrustumKeyPressed)
        {
            toggleViewFrustumKeyPressed = true;

            if (CameraComponent* camera = GetCamera())
            {
                camera->SetViewFrustumCulling(!camera->IsViewFrustumCullingEnabled());
            }
        }
    }
    else
    {
        toggleViewFrustumKeyPressed = false;
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_Q))
    {
        if (!debugContainerKeyPressed)
        {
            debugContainerEnabled = !debugContainerEnabled;
            debugContainerKeyPressed = true;

            if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
            {
                userInterfaceScene->EnableDebugContainer(debugContainerEnabled);
            }
        }
    }
    else
    {
        debugContainerKeyPressed = false;
    }

    if (debugContainerEnabled)
    {
        if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
        {
            std::string objectsRendered = "unk";
            std::string objectsCulled = "unk";

            if (CameraComponent* camera = GetCamera())
            {
                objectsRendered = std::to_string(camera->ObjectsRendered);
                objectsCulled = std::to_string(camera->ObjectsCulled);
            }

            userInterfaceScene->SetObjectsRenderedText(objectsRendered);
            userInterfaceScene->SetObjectsCulledText(objectsCulled);

            std::string fps = std::to_string((int)round(sTime.GetFPS()));
            userInterfaceScene->SetFPSText(fps);
        }
    }
}

void GameScene::UpdateCredits()
{
    if (auto userInterfaceScene = sSceneManager.GetScene<UserInterfaceScene>())
    {
        userInterfaceScene->ScrollCredits(40.f * sTime.GetUnscaledDeltaTime());
    }
}
