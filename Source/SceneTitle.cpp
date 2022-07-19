#include "SceneTitle.h"
#include "SceneManager.h"

#include "Input/Input.h"


/* main method */

void SceneTitle::Initialize()
{
    const DirectX::XMFLOAT2 screen = { Graphics::Instance().GetScreenWidth(),Graphics::Instance().GetScreenHeight() };

    menu_ = std::make_unique<MenuBar>(DirectX::XMFLOAT2(1,2));

    //↓仮設
    menu_->SetBG(new Texture(titleFile,1));
    menu_->SetLogo(new Texture(
        logoFile,1
        , { screen.x / 2,screen.y / 4 }
        , SPRITE_PIVOT::CM
    ));
    menu_->AddSubBer(new Texture(
        barFile,2
        , { screen.x / 2,screen.y * 3 / 5 }
        , SPRITE_PIVOT::CM
        , { 0.5f,0.5f }
    ), []() {
    SceneManager::Instance().changeScene(SCENE_TYPE::GAME);
});
    menu_->AddSubBer(new Texture(
        barFile,2
        , { screen.x / 2,screen.y * 4 / 5 }
        , SPRITE_PIVOT::CM
        , { 0.5f,0.5f }
    ), []() {
    //ウィンドウを閉じる処理をかく
        PostQuitMessage(WM_DESTROY);
});
    menu_->AddTexture(new Texture(
        texFile0,1
    ));

    // 音の初期化・再生
    BGM = Audio::Instance().LoadAudioSource("Data/Audio/TitleBGM.wav");
    BGM->Play(true);
}

void SceneTitle::Finalize()
{
    BGM.reset();
}

void SceneTitle::Update(float elapsedTime)
{
    // change scene
    menu_->Update();
}

void SceneTitle::Render()
{
    menu_->Render();
}

/* get type */

const SCENE_TYPE SceneTitle::getType() { return SCENE_TYPE::TITLE; }

