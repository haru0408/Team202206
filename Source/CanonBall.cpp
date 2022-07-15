#include "CanonBall.h"
#include "CanonBallManager.h"
#include "Canon.h"

CanonBall::CanonBall()
{
    model = new Model("Data/Model/Canon/CanonBall.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.005f;
}

CanonBall::~CanonBall()
{
    delete model;
}

// 更新処理
void CanonBall::Update(float elapsedTime)
{
    // 座標更新
    position.x += velocity.x;
    position.z += velocity.z;

    // 一定時間で削除する
    if (removeTimer <= 0) Destroy();
    --removeTimer;

    // 行列更新処理
    UpdateTransform(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
                    DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
                    DirectX::XMFLOAT3(0.0f, 0.25f, 0.0f));

    // モデル行列を更新
    model->UpdateTransform(transform);
}

// 行列更新処理
void CanonBall::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
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