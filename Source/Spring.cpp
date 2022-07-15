#include "Spring.h"
#include "Graphics/Graphics.h"

Spring::Spring(DirectX::XMFLOAT3 Length) : length(Length)
{
    model = new Model("Data/Model/Spring/Spring.mdl");

    scale = { 0.005f * length.x, 0.005f * length.y, 0.005f * length.z };
}

Spring::~Spring()
{
    delete model;
}

// 行列更新処理
void Spring::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
    const DirectX::XMFLOAT3& AdjustAngle,
    const DirectX::XMFLOAT3& AdjustPosition)
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x + AdjustScale.x,
                                                  scale.y + AdjustScale.y,
                                                  scale.z + AdjustScale.z);

    // 回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x + AdjustAngle.x,
                                                                angle.y + AdjustAngle.y,
                                                                angle.z + AdjustAngle.z);

    // 位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + AdjustPosition.x,
                                                       position.y + AdjustPosition.y,
                                                       position.z + AdjustPosition.z);

    // ３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

// デバッグプリミティブ描画
void Spring::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバッグ円柱を描画
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawBox(position, length.x, length.y, length.z, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// 更新処理
void Spring::Update(float elapsedTime)
{
    UpdateTransform(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 
        DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));

    model->UpdateTransform(transform);
}

// 描画処理
void Spring::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}