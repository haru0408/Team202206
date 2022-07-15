#include "Stage.h"

//　コンストラクタ
Stage::Stage()
{
    // ステージモデルを読み込み
    model[0] = new Model("Data/Model/Stage/Floor_0.mdl");
    model[1] = new Model("Data/Model/Stage/Floor_1.mdl");
    model[2] = new Model("Data/Model/Stage/Floor_2.mdl");
    model[3] = new Model("Data/Model/Stage/Floor_3.mdl");
    model[4] = new Model("Data/Model/Stage/Floor_4.mdl");
    model[5] = new Model("Data/Model/Stage/Floor_test.mdl");

    for (int i = 0; i < 6; i++)
    {
        position[i].y = { 20.0f * i };
    }


    scale.x = scale.z = 0.005f;
    scale.y = 0.0035;
}

Stage::~Stage()
{
    // ステージモデルを破棄
    for (int i = 0; i < 6; i++)
    {
        delete model[i];
    }
}

// 行列更新処理
void Stage::UpdateTransform()
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    // 回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 位置行列を作成
    for (int i = 0; i < 6; i++)
    {
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

        // 3つの行列を組み合わせ、ワールド行列を作成
        DirectX::XMMATRIX W = S * R * T;

        // 計算したワールド行列を取り出す
        DirectX::XMStoreFloat4x4(&transform[i], W);
    }
}

// 更新処理
void Stage::Update(float elapsedTime)
{
    UpdateTransform();

    for (int i = 0; i < 6; i++)
    {
        model[i]->UpdateTransform(transform[i]);
    }
  
}

// 描画処理
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // シェーダーにモデルを描画してもらう
    for (int i = 0; i < 6; i++)
    {
        shader->Draw(dc, model[i]);
    }
}