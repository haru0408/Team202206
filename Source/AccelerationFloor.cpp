#include "AccelerationFloor.h"

AccelerationFloor::AccelerationFloor(int judge)
{
    model = new Model("Data/Model/Cube/KasokuYuka.mdl");

    // モデルが大きいのでスケーリング
    scale.x = 0.0035f;
    scale.z = 0.01f;
    scale.y = 0.2;

    switch (judge)
    {
    case 0:
        angle.y = DirectX::XMConvertToRadians(0);
        break;
    case 1:
        angle.y = DirectX::XMConvertToRadians(-90);
        break;
    case 2:
        angle.y = DirectX::XMConvertToRadians(180);
        break;
    case 3:
        angle.y = DirectX::XMConvertToRadians(90);
        break;
    }

    // 幅、高さ設定
    height = 1.0f;
    floor_num = 1;
    floor_direction = judge;
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

DirectX::XMFLOAT3 AccelerationFloor::SetImpulse(int judge)
{
    switch (judge)
    {
    case 0:
        return impulse = {10,0,0};
        break;
    case 1:
        return impulse = { 0,0,10 };
        break;
    case 2:
        return impulse = { -10,0,0 };
        break;
    case 3:
        return impulse = { 0,0,-10 };
        break;
    }
}