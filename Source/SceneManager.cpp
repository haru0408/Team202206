#include "SceneManager.h"

#include "BaseScene.h"

#include "SceneDebug.h"
#include "SceneTitle.h"
#include "SceneGame.h"


Scene* SceneManager::scene = &SceneDebug::Instance();


/* main method */

void SceneManager::Finalize() { scene->Finalize(); }

void SceneManager::Update(float elapsedTime)
{
    isChange_ = false;
    scene->Update(elapsedTime);
}

void SceneManager::Render()
{
    if (isChange_)return;
    scene->Render();
}

/* change scene */

void SceneManager::changeScene(SCENE_TYPE next)
{
    assert(next != SCENE_TYPE::DEBUG);

    isChange_ = true;

    scene->Finalize();
    switch (next)
    {
    case SCENE_TYPE::TITLE:
        scene = &SceneTitle::Instance();
        break;
    case SCENE_TYPE::GAME:
        scene = &SceneGame::Instance();
        break;
    }
    scene->Initialize();
}
