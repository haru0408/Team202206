#include "Floor.h"
#include "Graphics/Graphics.h"
#include "FloorManager.h"
void Floor::UpdateTransform()
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    // 回転行列を作成

    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // 3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    // 計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

// デバッグプリミティブ描画
void Floor::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバッグ円柱を描画
    if(floor_num==0)debugRenderer->DrawBox(position, 2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(0, 1, 0, 1));
    if (floor_num == 1)debugRenderer->DrawBox(position,2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
    if (floor_num == 2)debugRenderer->DrawBox(position, 2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
}

void Floor::Destroy()
{
    FloorManager::Instance().Remove(this);
}