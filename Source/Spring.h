#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
class Spring
{
public:
	Spring(DirectX::XMFLOAT3 Length);
	~Spring();

	DirectX::XMFLOAT3 Revers_Velo();

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 行列更新処理
    void UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
        const DirectX::XMFLOAT3& AdjustAngle,
        const DirectX::XMFLOAT3& AdjustPosition);

    //デバッグ
    void DrawDebugPrimitive();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 長さ取得
    const DirectX::XMFLOAT3& GetLength() const { return length; }

    // 長さ設定
    void SetLength(const DirectX::XMFLOAT3& length) { this->length = length; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3     position = { 0, 0, 0 };
    DirectX::XMFLOAT3     angle = { 0, 0, 0 };
    DirectX::XMFLOAT3     scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4   transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    DirectX::XMFLOAT3     length = { 0.0f, 0.0f, 0.0f };
};
