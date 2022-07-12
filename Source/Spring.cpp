#include "Spring.h"
#include "Graphics/Graphics.h"
Spring::Spring()
{

}
Spring::~Spring()
{

}

void Spring::UpdateTransform()
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

// �f�o�b�O�v���~�e�B�u�`��
void Spring::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�O�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
    debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// �X�V����
void Spring::Update(float elapsedTime)
{

}

// �`�揈��
void Spring::Render(ID3D11DeviceContext* dc, Shader* shader)
{

}