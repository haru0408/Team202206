#include "Hole.h"
#include "HoleManager.h"
#include "Graphics/Graphics.h"

// 更新処理
void Hole::Update(float elapsedTime)
{

}

// デバッグプリミティブ描画
void Hole::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
    debugRenderer->DrawCylinder(position, radius * 0.75f, height, DirectX::XMFLOAT4(0, 1, 0, 1));
}

// 破棄
void Hole::Destroy()
{
    HoleManager::Instance().Remove(this);
}