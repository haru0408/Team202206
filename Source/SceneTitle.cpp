#include "SceneTitle.h"
#include "SceneManager.h"

#include "Input/Input.h"


TitleSprite::TitleSprite(const char* filename, const DirectX::XMFLOAT2& pos, SPRITE_PIVOT pivot, const DirectX::XMFLOAT2& scale, const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle, const DirectX::XMFLOAT4& color)
    :Sprite(filename)
{
    position_ = pos;
    pivot_ = pivot;
    scale_ = scale;
    texPos_ = texPos;
    if (texSize.x * texSize.y == 0.0f)
        texSize_ = { static_cast<float>(GetTextureWidth()),static_cast<float>(GetTextureHeight()) };
    else
        texSize_ = texSize;
    angle_ = angle;
    color_ = color;
}

void TitleSprite::Render()
{
    auto dc = Graphics::Instance().GetDeviceContext();

    auto pos = GetPosition();
    switch (pivot_)
    {
    case SPRITE_PIVOT::LT: /*nothing*/ break;
    case SPRITE_PIVOT::CT: pos.x -= texSize_.x * scale_.x / 2;                                      break;
    case SPRITE_PIVOT::RT: pos.x -= texSize_.x * scale_.x;                                          break;
    case SPRITE_PIVOT::LM:                                      pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::CM: pos.x -= texSize_.x * scale_.x / 2,  pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::RM: pos.x -= texSize_.x * scale_.x,      pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::LB:                                      pos.y -= texSize_.y * scale_.y;     break;
    case SPRITE_PIVOT::CB: pos.x -= texSize_.x * scale_.x / 2,  pos.y -= texSize_.y * scale_.y;     break;
    case SPRITE_PIVOT::RB: pos.x -= texSize_.x * scale_.x,      pos.y -= texSize_.y * scale_.y;     break;
    }

    Sprite::Render(dc
        , pos.x, pos.y
        , texSize_.x * scale_.x, texSize_.y * scale_.y
        , texPos_.x, texPos_.y
        , texSize_.x, texSize_.y
        , angle_
        , color_.x, color_.y, color_.z, color_.w
    );
}

//---------------------------------//
//---------------------------------//
//---------------------------------//

void MenuBar::Update() {
    // change scene
    {
        auto& input = Input::Instance().GetGamePad();

        //Bar選択
#if 1
        //AXIS LY
        {
            static bool up = false;
            const auto& axis = input.GetAxisLY();

            if (up == false) //trigger
                if (axis != 0.0f)
                {
                    //増減
                    if (axis <= -0.75f)
                        --index_;
                    else if (0.75f <= axis)
                        ++index_;

                    //clamp制御
                    if (loop_)
                    {
                        if (index_ < 0) index_ = subBar_.size() - 1;
                        if (subBar_.size() - 1 < index_) index_ = 0;
                    }
                    else index_ = (std::min)((std::max)(0, index_), static_cast<int>(subBar_.size()) - 1);
                }
            up = (axis != 0.0f);
        }
#else
        //BUTTON
        {
            switch ((input.GetButtonDown() & (GamePad::BTN_UP | GamePad::BTN_DOWN)))
            {
            case (GamePad::BTN_UP):
                --index_;
                if (index_ < 0)
                    index_ = (loop_) ? subBar_.size() - 1 : 0; //loopオンなら一番下にする。オフなら最高値にする。
                break;
            case (GamePad::BTN_DOWN):
                ++index_;
                if (subBar_.size() <= index_)
                    index_ = (loop_) ? 0 : subBar_.size() - 1; //loopオンなら一番上にする。
                break;
            }
        }
#endif

        //change
        if (Input::Instance().GetGamePad().GetButtonDown()
            & GamePad::BTN_START)
            func_[index_];
    }

    // bar update
    {
        //はいぷろ見ながらつくる
        //subBar_[index_]->SetScale()
        for (auto& bar : subBar_)
        {
            bar->SetColor({ 0.70f,0.70f,0.70f,1 });
            if (bar == subBar_.at(index_)) bar->SetColor({ 1,1,1,1 });
        }
    }
}

void MenuBar::Render()
{
    auto dc = Graphics::Instance().GetDeviceContext();

    // BG
    if (BG_ != nullptr)
        BG_->Render();

    // subBar_
    for (auto& it : subBar_) 
        it->Render();

    // titleLogo_
    if (titleLogo_ != nullptr)
        titleLogo_->Render();
}

//---------------------------------//
//---------------------------------//
//---------------------------------//

/* main method */

inline void apa() {
    SceneManager::Instance().changeScene(SCENE_TYPE::GAME);
}
inline void apue() {
    //ウィンドウを閉じる処理をかく
}

void SceneTitle::Initialize()
{
    const DirectX::XMFLOAT2 screen = { Graphics::Instance().GetScreenWidth(),Graphics::Instance().GetScreenHeight() };

    menu_ = std::make_unique<MenuBar>(new MenuBar(true));

    //↓仮設
    menu_->SetBG(new TitleSprite(".\\Data\\Sprite\\Title.png"));
    menu_->SetTitleLogo(new TitleSprite(
        ".\\Data\\Sprite\\LoadingIcon.png"
        , { screen.x / 2,screen.y / 4 }
        , SPRITE_PIVOT::CM
    ));
    menu_->AddSubBer(new TitleSprite(
        ".\\Data\\Sprite\\Title.png"
        , { screen.x / 2,screen.y * 3 / 5 }
        , SPRITE_PIVOT::CM
        , { 0.5f,0.5f }
    ), apa);
    menu_->AddSubBer(new TitleSprite(
        ".\\Data\\Sprite\\Title.png"
        , { screen.x / 2,screen.y * 4 / 5 }
        , SPRITE_PIVOT::CM
        , { 0.5f,0.5f }
    ), apue);
}

void SceneTitle::Finalize()
{
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

