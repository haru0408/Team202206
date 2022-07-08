#include "CanonBall.h"
#include "CanonBallManager.h"
#include "Canon.h"

// �X�V����
void CanonBall::Update(float elapsedTime)
{
    // ���W�X�V
    position.x += velocity.x;
    position.z += velocity.z;

    // ��莞�Ԃō폜����
    if (removeTimer <= 0) Destroy();
    --removeTimer;
}

// �`�揈��
void CanonBall::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// �f�o�b�O�v���~�e�B�u�`��
void CanonBall::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 1, 1));
}

// �j��
void CanonBall::Destroy()
{
    CanonBallManager::Instance().Remove(this);
}