#include "NormalFloor.h"
#include "FloorManager.h"


NormalFloor::NormalFloor()
{
    model = new Model("Data/Model/Cube/Cube2.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.z = 0.015f;
    scale.y = 0.005;

    // ���A�����ݒ�
    height = 1.0f;
    floor_num = 2;
}

NormalFloor::~NormalFloor()
{
    delete model;
}

void NormalFloor::Update(float elapsedTime)
{
    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

void NormalFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{

}

