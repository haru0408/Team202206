#pragma once
/*
* Fuji:
*  class     風エリア
*  Manager   特殊エリア用Manager
* -Attention    ・Character::Velocity の getter/setter がありませんが記述しています。
*               　 >> void AreaManager::CollisionAreaVsCharacter
*/


#include <DirectXMath.h>

#include <d3d11.h>
//#include <map>
#include <vector>
#include "Graphics/Graphics.h"
#include "Character.h"


class AreaWindow
{
public:
    // constructer etc.
    AreaWindow(const DirectX::XMFLOAT3& position,const DirectX::XMFLOAT3& radius, float pushPower = 3.0f)
        :pushValue_(pushPower),radius_(radius),position_(position)
    { }
    
    //main method
    void Update(float elapsedTime) {}
    void Render(ID3D11RenderTargetView* dc, Shader* shader) {}
    //debug
    void DrawDebugPrimitive();

    //getter
    const DirectX::XMFLOAT3& GetPosition()const { return position_; }
    const DirectX::XMFLOAT3& GetRadius()const { return radius_; }
    float GetPower()const { return pushValue_; }
    //setter
    const DirectX::XMFLOAT3& SetRadius()const { return radius_; }


private:
    DirectX::XMFLOAT3 position_ = {};
    DirectX::XMFLOAT3 radius_ = {};
    float pushValue_ = 0.0f;
};

class AreaManager
{
private:
    // constructer etc.
    AreaManager() {}
    ~AreaManager() { Clear(); }
public:
    // instance getter
    static AreaManager& Instance()
    {
        static AreaManager instance;
        return instance;
    }

    /* main method */
    void Update(float elapsedTime);
    void Render(ID3D11RenderTargetView* dc, Shader* shader);
    void Clear();
    //
    void CollisionAreaVsCharacter(Character* obj); //

    /* register, delete */
    void Register(AreaWindow* area);
    void Remove(AreaWindow* area);

    /* debug */
    void DrawDebugPrimitive();

    //getter
    int GetTotalCount()const { return static_cast<int>(windows_.size()); }
    AreaWindow* GetAreaWindow(int index) { return windows_.at(index); }


private:
    std::vector<AreaWindow*> windows_{};
    std::vector<AreaWindow*> removes_w_{};
};