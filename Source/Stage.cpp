#include "Stage.h"

//　コンストラクタ
Stage::Stage()
{
    // ステージモデルを読み込み
    model = new Model("Data/Model/Stage/Stage_1.mdl");
    scale.x = scale.z = 0.005f;
    scale.y = 0.0035;
    position.y = -1;
}

Stage::~Stage()
{
    // ステージモデルを破棄
    delete model;
}

// 行列更新処理
void Stage::UpdateTransform()
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

// 更新処理
void Stage::Update(float elapsedTime)
{
    UpdateTransform();

    model->UpdateTransform(transform);
}

// 描画処理
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}