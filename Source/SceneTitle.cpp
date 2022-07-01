#include "SceneTitle.h"
#include "SceneManager.h"

#include "Input/Input.h"

TitleSprite::TitleSprite(const char* filename, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& scale, const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle, const DirectX::XMFLOAT4& color)
    :Sprite(filename)
{
    position_ = pos;
    scale_ = scale;
    texPos_ = texPos;
    if (texSize.x * texSize.y == 0.0f)
        texSize_ = { static_cast<float>(GetTextureWidth()),static_cast<float>(GetTextureHeight())};
    else
        texSize_ = texSize;
    angle_ = angle;
    color_ = color;
}

//---------------------------------//
//---------------------------------//
//---------------------------------//

/* main method */

void SceneTitle::Initialize()
{
    sprites_.emplace_back(new TitleSprite(".\\Data\\Sprite\\Title.png"));
}

void SceneTitle::Finalize()
{
    for (auto it : sprites_)
    {
        if (it != nullptr)
            delete it;
    }
    sprites_.clear();
}

void SceneTitle::Update(float elapsedTime)
{
    // change scene
    if (Input::Instance().GetGamePad().GetButtonDown()
        & GamePad::BTN_START)
        SceneManager::Instance().changeScene(SCENE_TYPE::GAME);
}

void SceneTitle::Render()
{
    auto dc = Graphics::Instance().GetDeviceContext();
    for (auto& it : sprites_)
    {
        auto& pos = it->GetPosition();
        auto& scale = it->GetScale();
        auto& texPos = it->GetTexPos();
        auto& texSize = it->GetTexSize();
        auto angle = it->GetAngle();
        auto& color = it->GetColor();
        it->Render(dc
            , pos.x, pos.y
            , texSize.x * scale.x, texSize.y * scale.y
            , texPos.x, texPos.y
            , texSize.x, texSize.y
            , angle
            , color.x, color.y, color.z, color.w
        );
    }
}

/* get type */

const SCENE_TYPE SceneTitle::getType() { return SCENE_TYPE::TITLE; }

