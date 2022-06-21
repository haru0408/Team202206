#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"


// �X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    for (Enemy* enemy : removes)
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(),
            enemies.end(),
            enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        delete enemy;
    }
    removes.clear();

    // �G���m�̏Փˏ���
    CollisionEnemyVsEnemies();
}

// �`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    if (enemy != nullptr)
    {
        enemies.emplace_back(enemy);
    }
}

// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy; //enemies���w���f�[�^������
        }
    }

    enemies.clear(); //enemies���̂��̂�����
}

void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = enemies.at(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemies.at(j);

            DirectX::XMFLOAT3 outPosition;
#if 0
            if (Collision::IntersectSphereVsSphere(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                outPosition
            ))
            {
                enemyB->SetPosition(outPosition);
            }
#endif // 0
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outPosition
            ))
            {
                enemyB->SetPosition(outPosition);
            }

        }
    }
}

void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}