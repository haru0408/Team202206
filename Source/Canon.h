#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"

class Canon
{
public:
     Canon() {}
    ~Canon() {}

    // 更新処理
    void Update(float elapsedTime);

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

    // 長さ取得
    const DirectX::XMFLOAT3& GetLength() const { return length; }

    // 長さ設定
    void SetLength(const DirectX::XMFLOAT3& length) { this->length = length; }

    // 向き取得
    const int& GetDirection() const { return direction; }

    // 上向き設定
    void SetUpDirection()    { this->direction = 1; }
    // 下向き設定
    void SetDownDirection()  { this->direction = 2; }
    // 左向き設定
    void SetLeftDirection()  { this->direction = 3; }
    // 右向き設定
    void SetRightDirection() { this->direction = 4; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 length   = { 1.0f, 1.0f, 1.0f };

    // 向き判定用変数
    // 1:上向き
    // 2:下向き
    // 3:左向き
    // 4:右向き
    int direction = 0;

    // 発射間隔時間
    int LanchTimer = 60;
};