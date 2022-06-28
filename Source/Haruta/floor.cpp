#include "floor.h"
void Destroy_timer(Enemy* enemy, int i)
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemy = enemyManager.GetEnemy(i);

    static float timer = 0;
    timer++;
    if (timer / 60 > 3)
    {
        enemy->Destroy();
        timer = 0;
    }
}

