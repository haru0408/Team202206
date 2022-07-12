#include "AccelerationFloor.h"

AccelerationFloor::AccelerationFloor()
{
    model = new Model("Data/Model/Cube/Cube2.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.z = 0.015f;
    scale.y = 0.005;

    // 幅、高さ設定
    height = 1.0f;
    floor_num = 1;
}

AccelerationFloor::~AccelerationFloor()
{
    delete model;
}

void AccelerationFloor::Update(float elapsedTime)
{
    // オブジェクト行列を更新
    UpdateTransform();

    // モデル行列更新
    model->UpdateTransform(transform);
}

void AccelerationFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

DirectX::XMFLOAT3 AccelerationFloor::SetImpulse()
{
    DirectX::XMFLOAT3 impulse = {500,500,500};


    return impulse;
}