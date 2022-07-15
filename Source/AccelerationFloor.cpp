#include "AccelerationFloor.h"

AccelerationFloor::AccelerationFloor(int judge)
{
    model = new Model("Data/Model/Cube/KasokuYuka.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
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

    // ���A�����ݒ�
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
    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    // ���f���s��X�V
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