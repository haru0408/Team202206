#include "Canon.h"
#include "CanonManager.h"
#include "CanonBallManager.h"

Canon::Canon()
{
    model = new Model("Data/Model/Canon/Canon.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
}

Canon::~Canon()
{
    delete model;
}

// 更新処理
void Canon::Update(float elapsedTime)
{
    CanonBallManager& canonBallManager = CanonBallManager::Instance();

    // 弾発射
    if (LanchTimer < 0)
    {
        CanonBall* canonBall = new CanonBall();
        canonBall->SetPosition(DirectX::XMFLOAT3(position.x, position.y + (length.y * 0.35f), position.z));
        canonBallManager.Register(canonBall);

        LanchTimer = 60;
    }
    --LanchTimer;

    // 弾の速度を設定する
    const float BallSpeed = 0.2f;
    int canonBallCount    = canonBallManager.GetCanonBallCount();
    for (int i = 0; i < canonBallCount; ++i)
    {
        CanonBall* canonBall = canonBallManager.GetCanonBall(i);

        if (canonBall->GetVelocity().x == 0.0f && canonBall->GetVelocity().z == 0.0f)
        {
            switch (direction)
            {
            case 1: // 上向き
                canonBall->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, BallSpeed));

                AdjustPosition = { 0.0f, 1.0f, -0.5f };
                DirectionAngle = DirectX::XMConvertToRadians(0.0f);

                break;

            case 2: // 下向き
                canonBall->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, -BallSpeed));

                AdjustPosition = { 0.0f, 1.0f, 0.5f };
                DirectionAngle = DirectX::XMConvertToRadians(180.0f);

                break;

            case 3: // 左向き
                canonBall->SetVelocity(DirectX::XMFLOAT3(-BallSpeed, 0.0f, 0.0f));

                AdjustPosition = { 0.5f, 1.0f, 0.0f };
                DirectionAngle = DirectX::XMConvertToRadians(-90.0f);

                break;

            case 4: // 右向き
                canonBall->SetVelocity(DirectX::XMFLOAT3(BallSpeed, 0.0f, 0.0f));

                AdjustPosition = { -0.5f, 1.0f, 0.0f };
                DirectionAngle = DirectX::XMConvertToRadians(90.0f);

                break;
            }
        }
    }

    // 行列更新処理
    UpdateTransform(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
                    DirectX::XMFLOAT3(0.0f, DirectionAngle, 0.0f), 
                    AdjustPosition);

    // モデル行列を更新
    model->UpdateTransform(transform);
}

// 行列更新処理
void Canon::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
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
void Canon::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// デバッグプリミティブ描画
void Canon::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawBox(position, length.x, length.y, length.z, DirectX::XMFLOAT4(1, 0, 1, 1));
}

// 破棄
void Canon::Destroy()
{
    CanonManager::Instance().Remove(this);
}