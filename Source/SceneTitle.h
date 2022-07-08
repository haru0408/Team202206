#pragma once
/*
* Fuji:
*  class     SceneTitle, 2DâÊëú
*  Manager   - Nothing -
* -Attention    - Nothing -
*/


#include "BaseScene.h"

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
//#include "Stage.h"
//#include "Player.h"
//#include "CameraController.h"

enum class SPRITE_PIVOT {
    LT,CT,RT
    ,LM,CM,RM
    ,LB,CB,RB
};
class TitleSprite :public Sprite
{
public:
    // constructer etc.
    TitleSprite(
        const char* filename
        , const DirectX::XMFLOAT2& pos = {}
        , SPRITE_PIVOT pivot = SPRITE_PIVOT::LT
        , const DirectX::XMFLOAT2& scale = { 1,1 }
        , const DirectX::XMFLOAT2& texPos = {}
        , const DirectX::XMFLOAT2& texSize = {}
        , float angle = 0.0f
        , const DirectX::XMFLOAT4& color = { 1,1,1,1 }
    );
    TitleSprite(TitleSprite&) = delete;

    /* main method */
    void Render();

    //getter
    const DirectX::XMFLOAT2& GetPosition()const { return position_; }
    const DirectX::XMFLOAT2& GetScale()const { return scale_; }
    const DirectX::XMFLOAT2& GetTexPos()const { return texPos_; }
    const DirectX::XMFLOAT2& GetTexSize()const { return texSize_; }
    float GetAngle()const { return angle_; }
    const DirectX::XMFLOAT4& GetColor()const { return color_; }

    //setter
    void SetScale(const DirectX::XMFLOAT2& s) { scale_ = s; }
    void SetColor(const DirectX::XMFLOAT4& c) { color_ = c; }
    void SetAngle(float a) { angle_ = normalizeAngle(a); }
    void AddAngle(float a) { normalizeAngle(angle_ += a); }

private:
    DirectX::XMFLOAT2 position_{};
    SPRITE_PIVOT pivot_{};
    DirectX::XMFLOAT2 scale_{};
    DirectX::XMFLOAT2 texPos_{};
    DirectX::XMFLOAT2 texSize_{};
    float angle_{};
    DirectX::XMFLOAT4 color_{};
};

class MenuBar
{
public:
    //constructer etc.
    MenuBar(bool loop):loop_(loop) {}
    ~MenuBar() {
        if (titleLogo_) delete titleLogo_;
        if (BG_) delete BG_;
        for (auto it : subBar_)
        {
            if (it != nullptr)
                delete it;
        }
        subBar_.clear();
    }

    /* main method */
    void Update();
    void Render();

    //setter
    void SetTitleLogo(TitleSprite* s) { titleLogo_ = s; }
    void SetBG(TitleSprite* s) { BG_ = s; }
    //
    void AddSubBer(TitleSprite* s, void* func) { subBar_.emplace_back(s); func_.emplace_back(func); }

private:
    TitleSprite* titleLogo_ = nullptr;
    TitleSprite* BG_ = nullptr;
    std::vector<TitleSprite*> subBar_{};
    std::vector<void*> func_{}; //BarñàÇÃèàóùäiî[å…

    bool loop_ = false;
    int index_ = 0;
    bool islarge_ = false; //ç°ÇÕégÇ¡ÇƒÇ»Ç¢
};



class SceneTitle:public Scene
{
public:
    /* instance getter */
    static SceneTitle& Instance() {
        static SceneTitle instance;
        return instance;
    }

    //---------------------------//
private:
    // constructer etc.
    SceneTitle() {}
    ~SceneTitle()override {}

    /* main method */
    void Initialize()override;
    void Finalize()override;
    void Update(float elapsedTime)override;
    void Render()override;

    /* get type */
    const SCENE_TYPE getType();

private:
    std::unique_ptr<MenuBar> menu_ = nullptr;
};

