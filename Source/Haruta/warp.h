#pragma once
#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

using namespace DirectX;

void Destroy_timer(Enemy* enemy, int i);

class Warp
{
public:
    Warp();
    ~Warp() {};

    void warp(XMFLOAT3 warp_pos);
    void warp_hit(XMFLOAT3 player_pos);
    FLOAT warp_move(XMFLOAT3 player_pos,float velocity, float speed, float height);

    bool hit = false;
    XMFLOAT3 warp_pos[6];

private:
    XMFLOAT3 pos = {};
    XMFLOAT3 size = {};
    XMFLOAT4 color = {};
};
