#include "Floor.h"
#include "Graphics/Graphics.h"
#include "FloorManager.h"
void Floor::UpdateTransform()
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
void Floor::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�O�~����`��
    if(floor_num==0)debugRenderer->DrawBox(position, 2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(0, 1, 0, 1));
    if (floor_num == 1)debugRenderer->DrawBox(position,2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
    if (floor_num == 2)debugRenderer->DrawBox(position, 2.0f, 1.0f, 2.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
}

void Floor::Destroy()
{
    FloorManager::Instance().Remove(this);
}