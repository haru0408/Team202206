#include "NormalFloor.h"
#include "FloorManager.h"


NormalFloor::NormalFloor()
{
    model = new Model("Data/Model/Cube/Cube2.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.z = 0.015f;
    scale.y = 0.005;

    // 幅、高さ設定
    height = 1.0f;
    floor_num = 2;
}

NormalFloor::~NormalFloor()
{
    delete model;
}

void NormalFloor::Update(float elapsedTime)
{
    // オブジェクト行列を更新
    UpdateTransform();

    // モデル行列更新
    model->UpdateTransform(transform);
}

void NormalFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{

}

