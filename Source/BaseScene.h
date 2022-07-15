#pragma once
/*
* Fuji:
*  class     Sceneのベース
*  Manager   - Nothing -
* -Attention    - Nothing -
*/

#include <DirectXMath.h>
#include <vector>

#include "Audio/Audio.h"

enum class SCENE_TYPE;
class Scene
{
protected:
    // constructer etc.
    Scene() {}
    virtual ~Scene() {}
public:
    /* main method */
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;

    /* get type */
    virtual const SCENE_TYPE getType() = 0;
};

//------------------------//

#include "Graphics/Sprite.h"
enum class SPRITE_PIVOT {
    LT,CT,RT
    ,LM,CM,RM
    ,LB,CB,RB
};
class Texture :public Sprite
{
public:
    // constructer etc.
    Texture(
        const char* filename
        , const DirectX::XMFLOAT2& pos = {}
        , SPRITE_PIVOT pivot = SPRITE_PIVOT::LT
        , const DirectX::XMFLOAT2& scale = { 1,1 }
        , int zCode = 0
        , const DirectX::XMFLOAT2& texPos = {}
        , const DirectX::XMFLOAT2& texSize = {}
        , float angle = 0.0f
        , const DirectX::XMFLOAT4& color = { 1,1,1,1 }
    );
    Texture(Texture&) = delete;

    /* main method */
    /// <param name="p">基準点</param>
    void Render(const DirectX::XMFLOAT2& p);

    //getter
    const DirectX::XMFLOAT2& GetPosition()const { return position_; }
    const DirectX::XMFLOAT2& GetScale()const { return scale_; }
    const DirectX::XMFLOAT2& GetTexPos()const { return texPos_; }
    const DirectX::XMFLOAT2& GetTexSize()const { return texSize_; }
    float GetAngle()const { return angle_; }
    const DirectX::XMFLOAT4& GetColor()const { return color_; }
    int GetZ()const { return zCode_; }

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
    int zCode_ = 0;
private:
    inline float normalizeAngle(float& angle)
    {
        if (angle < -DirectX::XM_PI) angle += DirectX::XM_2PI;
        if (DirectX::XM_PI < angle) angle -= DirectX::XM_2PI;
        return angle;
    }
};

class MenuBar
{
public:
    //constructer etc.
    /// <summary>
    /// 
    /// </summary>
    /// <param name="size">矩形整列用</param>
    /// <param name="vertical">矩形整列時上下にバー移動できるか否か</param>
    /// <param name="horizontal">矩形整列時左右限界を移動できるか否か</param>
    MenuBar(const DirectX::XMFLOAT2& size, bool loop = true, bool horizontal = true, bool vertical = true)
        :loop_(loop), size_(size), moveVertical_(vertical), moveHorizontal_(horizontal)
    {}
    ~MenuBar() {
        if (logo_) delete logo_;
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
    void SetLogo(Texture* s) { logo_ = s; }
    void SetBG(Texture* s) { BG_ = s; }
    void SetPosition(const DirectX::XMFLOAT2& p) { position_ = p; }
    //
    void AddSubBer(Texture* s, void* func) { subBar_.emplace_back(s); func_.emplace_back(func); }
    void AddTexture(Texture* s) { textures_.emplace_back(s); }
private:
    DirectX::XMFLOAT2 position_ = {};

    Texture* logo_ = nullptr;
    Texture* BG_ = nullptr;
    std::vector<Texture*> textures_{};
    std::vector<Texture*> subBar_{};
    std::vector<void*> func_{}; //Bar毎の処理格納庫

    bool loop_ = false;     // 限界を超える操作をしたときに反対の限界値にいくか
    int index_ = 0;         // 選択中のバー番号
    DirectX::XMFLOAT2 size_ = { 1,1 };  // バーの行列数
    bool moveVertical_ = false;         // 矩形整列時など選択バーの上下移動可不可
    bool moveHorizontal_ = false;       // 矩形整列時など選択バーの左右限界移動可不可
    bool sortHorizontal_ = false;       // バー並べの優先方向。未使用。面倒くさいから描画位置で誤魔化してちょ
    bool islarge_ = false;  // 今は使ってない。バーのｴﾌｪｸﾄｫｫｵ用だけどやる気ナス

};


