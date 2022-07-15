#include "Stage.h"

//�@�R���X�g���N�^
Stage::Stage()
{
    // �X�e�[�W���f����ǂݍ���
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
    // �X�e�[�W���f����j��
    for (int i = 0; i < 6; i++)
    {
        delete model[i];
    }
}

// �s��X�V����
void Stage::UpdateTransform()
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    // ��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // �ʒu�s����쐬
    for (int i = 0; i < 6; i++)
    {
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position[i].x, position[i].y, position[i].z);

        // 3�̍s���g�ݍ��킹�A���[���h�s����쐬
        DirectX::XMMATRIX W = S * R * T;

        // �v�Z�������[���h�s������o��
        DirectX::XMStoreFloat4x4(&transform[i], W);
    }
}

// �X�V����
void Stage::Update(float elapsedTime)
{
    UpdateTransform();

    for (int i = 0; i < 6; i++)
    {
        model[i]->UpdateTransform(transform[i]);
    }
  
}

// �`�揈��
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // �V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    for (int i = 0; i < 6; i++)
    {
        shader->Draw(dc, model[i]);
    }
}