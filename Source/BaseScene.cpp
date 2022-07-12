#include "BaseScene.h"

#include <list>

#include "Graphics/Graphics.h"
#include "Input/Input.h"


Texture::Texture(const char* filename, const DirectX::XMFLOAT2& pos, SPRITE_PIVOT pivot, const DirectX::XMFLOAT2& scale, int zCode, const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle, const DirectX::XMFLOAT4& color)
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
void Texture::Render(const DirectX::XMFLOAT2& p)
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
    pos.x += p.x, pos.y += p.y;

    Sprite::Render(dc
        , pos.x, pos.y
        , texSize_.x * scale_.x, texSize_.y * scale_.y
        , texPos_.x, texPos_.y
        , texSize_.x, texSize_.y
        , angle_
        , color_.x, color_.y, color_.z, color_.w
    );
}

void MenuBar::Update() {
    // change scene
    {
        auto& input = Input::Instance().GetGamePad();
        int barSize = static_cast<int>(subBar_.size());

        //Bar選択
        //AXIS LX
        {
            static bool BarLX_up = false;
            const auto& axis = input.GetAxisLX();

            if (BarLX_up == false) //trigger
                if (axis != 0.0f)
                {
                    int old = index_;

                    //増減
                    if (axis <= -0.75f)
                        --index_;
                    else if (0.75f <= axis)
                        ++index_;

                    //左右限界うんぬん
                    if (moveHorizontal_ == false) {
                        int sx = static_cast<int>(size_.x);
                        if (old / sx != index_ / sx)
                            index_ = old;
                    }
                    //clamp制御
                    if (loop_)
                    {
                        if (index_ < 0) index_ = barSize - 1;
                        if (barSize - 1 < index_) index_ = 0;
                    }
                    else index_ = (std::min)((std::max)(0, index_), static_cast<int>(barSize) - 1);
                }
            BarLX_up = (axis != 0.0f);
        }
#if 1
        //AXIS LY
        {
            static bool BarLY_up = false;
            const auto& axis = input.GetAxisLY();

            if (BarLY_up == false) //trigger
                if (axis != 0.0f)
                {
                    //増減
                    if (axis <= -0.75f)
                        //--index_;
                        index_ = (moveVertical_)
                        ? index_ - size_.x
                        : index_--;
                    else if (0.75f <= axis)
                        index_ = (moveVertical_)
                        ? index_ + size_.x
                        : index_++;

                    //clamp制御
                    if (loop_)
                    {
                        if (index_ < 0) index_ = barSize - 1;
                        if (barSize - 1 < index_) index_ = 0;
                    }
                    else index_ = (std::min)((std::max)(0, index_), static_cast<int>(barSize) - 1);
                }
            BarLY_up = (axis != 0.0f);
        }
#else
        //BUTTON
        {
            switch ((input.GetButtonDown() & (GamePad::BTN_UP | GamePad::BTN_DOWN)))
            {
            case (GamePad::BTN_UP):
                --index_;
                if (index_ < 0)
                    index_ = (loop_) ? barSize - 1 : 0; //loopオンなら一番下にする。オフなら最高値にする。
                break;
            case (GamePad::BTN_DOWN):
                ++index_;
                if (barSize <= index_)
                    index_ = (loop_) ? 0 : barSize - 1; //loopオンなら一番上にする。
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

#if 1
    // BG
    if (BG_ != nullptr)
        BG_->Render(position_);

    // texture
    for (auto& it : textures_)
        it->Render(position_);

    // subBar_
    for (auto& it : subBar_)
        it->Render(position_);

    // titleLogo_
    if (logo_ != nullptr)
        logo_->Render(position_);
#else
    std::list<Texture, int> spr{};

    spr.emplace_back(logo_, logo_->GetZ());
    spr.emplace_back(BG_,BG_->GetZ());
    for (auto& it : textures_)
        spr.emplace_back(it,it->GetZ());
    for (auto& it : subBar_)
        spr.emplace_back(it,it->GetZ());

    spr.sort();
    for (auto& it : spr)
        it.Render(position_);
#endif

}

//---------------------------------//
