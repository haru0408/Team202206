#include "Hole.h"
#include "HoleManager.h"
#include "Graphics/Graphics.h"

// �X�V����
void Hole::Update(float elapsedTime)
{

}

// �f�o�b�O�v���~�e�B�u�`��
void Hole::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
    debugRenderer->DrawCylinder(position, radius * 0.75f, height, DirectX::XMFLOAT4(0, 1, 0, 1));
}

// �j��
void Hole::Destroy()
{
    HoleManager::Instance().Remove(this);
}