#include "CanonBall.h"
#include "CanonBallManager.h"
#include "Canon.h"

// 更新処理
void CanonBall::Update(float elapsedTime)
{
    // 座標更新
    position.x += velocity.x;
    position.z += velocity.z;

    // 一定時間で削除する
    if (removeTimer <= 0) Destroy();
    --removeTimer;
}

// 描画処理
void CanonBall::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// デバッグプリミティブ描画
void CanonBall::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 1, 1));
}

// 破棄
void CanonBall::Destroy()
{
    CanonBallManager::Instance().Remove(this);
}