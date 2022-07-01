#pragma once
/*
* Fuji:
*  class     SceneTitle, 2D‰æ‘œ
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


class TitleSprite:public Sprite
{
public:
    // constructer etc.
    TitleSprite(
        const char* filename
        , const DirectX::XMFLOAT2& pos = {}
        , const DirectX::XMFLOAT2& scale = { 1,1 }
        , const DirectX::XMFLOAT2& texPos = {}
        , const DirectX::XMFLOAT2& texSize = {}
        , float angle = 0.0f
        , const DirectX::XMFLOAT4& color = { 1,1,1,1 }
    );
    TitleSprite(TitleSprite&) = delete;

    //getter
    const DirectX::XMFLOAT2& GetPosition()const { return position_; }
    const DirectX::XMFLOAT2& GetScale()const { return scale_; }
    const DirectX::XMFLOAT2& GetTexPos()const { return texPos_; }
    const DirectX::XMFLOAT2& GetTexSize()const { return texSize_; }
    float GetAngle()const { return angle_; }
    const DirectX::XMFLOAT4& GetColor()const { return color_; }

    //setter
    void SetAngle(float a) { angle_ = normalizeAngle(a); }
    void AddAngle(float a) { normalizeAngle(angle_ += a); }

private:
    DirectX::XMFLOAT2 position_{};
    DirectX::XMFLOAT2 scale_{};
    DirectX::XMFLOAT2 texPos_{};
    DirectX::XMFLOAT2 texSize_{};
    float angle_{};
    DirectX::XMFLOAT4 color_{};
private:
    inline float normalizeAngle(float& angle)
    {
        if (angle < -DirectX::XM_PI) angle += DirectX::XM_2PI;
        if (DirectX::XM_PI < angle) angle -= DirectX::XM_2PI;
        return angle;
    }
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
    std::vector<TitleSprite*> sprites_{};
};