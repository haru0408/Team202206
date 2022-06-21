#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics//Graphics.h"

void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

#if 0
    // �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
#endif // 0
    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}