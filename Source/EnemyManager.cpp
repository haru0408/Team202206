#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"


// 更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    // 破棄処理
    for (Enemy* enemy : removes)
    {
        // std::vectorから要素を削除する場合はイテレータで削除しなければならない
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

    // 敵同士の衝突処理
    CollisionEnemyVsEnemies();
}

// 描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

// エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    if (enemy != nullptr)
    {
        enemies.emplace_back(enemy);
    }
}

// エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy; //enemiesが指すデータを消す
        }
    }

    enemies.clear(); //enemiesそのものを消す
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