#pragma once

#include <DirectXMath.h>

class Hole
{
public:
     Hole() { height = 1.5f; }
    ~Hole() {}

    // 更新処理
    void Update(float elapsedTime);

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 破棄
    void Destroy();

    // 位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 半径取得
    float GetRadius() const { return radius; }

    // 半径設定
    void SetRadius(const float& radius) { this->radius = radius; }

    // 高さ取得
    float GetHeight() const { return height; }

    // 高さ設定
    void SetHeight(const float& height) { this->height = height; }

private:
    DirectX::XMFLOAT3 position = {};

    float radius = 1.0f;
    float height = 3.0f;
};