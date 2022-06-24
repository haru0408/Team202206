#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics//Graphics.h"

void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

#if 0
    // 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
#endif // 0
    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}