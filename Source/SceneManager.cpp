#include "SceneManager.h"

#include "BaseScene.h"

#include "SceneDebug.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneClear.h"

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

    // 画面クリア＆レンダーターゲット設定
    {
        Graphics& graphics = Graphics::Instance();
        ID3D11DeviceContext* dc = graphics.GetDeviceContext();
        ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

        FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
        dc->ClearRenderTargetView(rtv, color);
        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        dc->OMSetRenderTargets(1, &rtv, dsv);
    }

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
    case SCENE_TYPE::CLEAR:
        scene = &SceneClear::Instance();
        break;
    }
    scene->Initialize();
}
