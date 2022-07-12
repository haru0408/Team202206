#include "DisappearingFloor.h"
#include "FloorManager.h"

DisappearingFloor::DisappearingFloor()
{
    model = new Model("Data/Model/Cube/Cube2.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.z = 0.015f;
    scale.y = 0.005;

    // ���A�����ݒ�
    height = 1.0f;
    floor_num = 0;
}

DisappearingFloor::~DisappearingFloor()
{
    delete model;
}

void DisappearingFloor::Update(float elapsedTime)
{
    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

void DisappearingFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}


void DisappearingFloor::Destroy_timer(Floor* floor, int i)
{
    FloorManager& floorManager = FloorManager::Instance();
    floor = floorManager.GetFloor(i);

    static float timer = 0;
    timer++;
    if (timer / 60 > 3)
    {
        floor->Destroy();
        timer = 0;
    }
}