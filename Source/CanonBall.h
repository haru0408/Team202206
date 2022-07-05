#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"

class CanonBall
{
public:
     CanonBall() {}
    ~CanonBall() {}

    // 更新処理
    void Update(float elapsedTime);

    // 速度更新処理
    void UpdateVelocity(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 破棄
    void Destroy();

    // 位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 速度取得
    const DirectX::XMFLOAT3& GetVelocity() const { return velocity; }

    // 速度設定
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }

    // 半径取得
    float GetRadius() const { return radius; }

    // 半径設定
    void SetRadius(const float& radius) { this->radius = radius; }

    // 高さ取得
    float GetHeight() const { return height; }

    // 高さ設定
    void SetHeight(const float& height) { this->height = height; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 velocity = {};

    float radius = 0.25f;
    float height = 0.5f;

    // 弾消滅時間
    int removeTimer = 120;
};