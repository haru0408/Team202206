#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"

class Floor
{
public:
    Floor() {}

    virtual ~Floor() {}

    void Update();

    void UpdateTransform();

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //デバッグ
    virtual void DrawDebugPrimitive();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    virtual void Destroy_timer(Floor* floor,int i) = 0;

    virtual DirectX::XMFLOAT3 SetImpulse() = 0;

    void SetFloorStage(int set) { this->Floor_Stage = set; };

    int GetFloorStage() { return Floor_Stage; };
    // 破棄
    void Destroy();

    int GetFloorNum() const { return floor_num; }
protected:
    DirectX::XMFLOAT3     position = { 0, 0, 0 };
    DirectX::XMFLOAT3     angle = { 0, 0, 0 };
    DirectX::XMFLOAT3     scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4   transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;

    int floor_num = 0;

    int Floor_Stage;
};
