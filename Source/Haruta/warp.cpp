#include "warp.h"

Warp::Warp()
{
    warp_pos[0] = { 17, 0.5f, -17 };
    warp_pos[1] = { -17,20.5f,-17 };
    warp_pos[2] = { 17, 40.5f,17 };
    warp_pos[3] = { -17,60.5f,-17 };
    warp_pos[4] = { 17, 80.5f, 17 };
    warp_pos[5] = { -17,100.5f,-17 };
}

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

void Warp::warp(XMFLOAT3 warp_pos) //‘ä‚Ì•`‰æ(Player::Update‚ÅŒÄ‚Ô)
{
    DebugRenderer* d = Graphics::Instance().GetDebugRenderer();
    pos = warp_pos;
    size = { 1.5f,0.3f,1.5f };
    color = { 1,1,0,1 };
    d->DrawBox(pos, size.x, size.y, size.z, color);
}

void Warp::warp_hit(XMFLOAT3 player_pos)
{
    DebugRenderer* d = Graphics::Instance().GetDebugRenderer();
    DirectX::XMFLOAT3 outPosition;

    if (Collision::IntersectBoxVsBox_Ground(
        player_pos,
        1.0f,
        1.0f,
        1.0f,
        pos,
        size.x,
        size.y,
        size.z,
        outPosition))
    {
        color = { 1,0,1,1 };
        d->DrawBox(pos, size.x, size.y, size.z, color);
        hit = true;
    }
}

FLOAT Warp::warp_move(XMFLOAT3 player_pos, float velocity, float speed, float height)
{
    velocity += speed;

    if (player_pos.y > height)
    {
        player_pos.y = height;
        hit = false;
    }

    return velocity;
}

