#pragma once
#include "Enemy.h"
#include "EnemyManager.h"
#include "Character.h"
#include <imgui.h>
#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Input/Input.h"
#include "Graphics//Graphics.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
void Destroy_timer(Enemy* enemy, int i);
void Vanishing_floor(Player* player);