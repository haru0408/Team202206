#include "Stage.h"

//�@�R���X�g���N�^
Stage::Stage()
{
    // �X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Stage/Stage_1.mdl");
    scale.x = scale.z = 0.005f;
    scale.y = 0.0035;
    position.y = -1;
}

Stage::~Stage()
{
    // �X�e�[�W���f����j��
    delete model;
}

// �s��X�V����
void Stage::UpdateTransform()
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    // ��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // 3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �X�V����
void Stage::Update(float elapsedTime)
{
    UpdateTransform();

    model->UpdateTransform(transform);
}

// �`�揈��
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    // �V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}