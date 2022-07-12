#include "AccelerationFloor.h"

AccelerationFloor::AccelerationFloor()
{
    model = new Model("Data/Model/Cube/Cube2.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.z = 0.015f;
    scale.y = 0.005;

    // ���A�����ݒ�
    height = 1.0f;
    floor_num = 1;
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

DirectX::XMFLOAT3 AccelerationFloor::SetImpulse()
{
    DirectX::XMFLOAT3 impulse = {500,500,500};


    return impulse;
}