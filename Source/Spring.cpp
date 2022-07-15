#include "Spring.h"
#include "Graphics/Graphics.h"

Spring::Spring(DirectX::XMFLOAT3 Length) : length(Length)
{
    model = new Model("Data/Model/Spring/Spring.mdl");

    scale = { 0.005f * length.x, 0.005f * length.y, 0.005f * length.z };
}

Spring::~Spring()
{
    delete model;
}

// �s��X�V����
void Spring::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
    const DirectX::XMFLOAT3& AdjustAngle,
    const DirectX::XMFLOAT3& AdjustPosition)
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x + AdjustScale.x,
                                                  scale.y + AdjustScale.y,
                                                  scale.z + AdjustScale.z);

    // ��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x + AdjustAngle.x,
                                                                angle.y + AdjustAngle.y,
                                                                angle.z + AdjustAngle.z);

    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + AdjustPosition.x,
                                                       position.y + AdjustPosition.y,
                                                       position.z + AdjustPosition.z);

    // �R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �f�o�b�O�v���~�e�B�u�`��
void Spring::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�O�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawBox(position, length.x, length.y, length.z, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// �X�V����
void Spring::Update(float elapsedTime)
{
    UpdateTransform(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 
        DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));

    model->UpdateTransform(transform);
}

// �`�揈��
void Spring::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}