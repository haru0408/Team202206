#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"

// プレイヤー
class Player : public Character
{
public:
    Player();
    ~Player() override;

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // デバッグ用GUI描画
    void DrawDebugGUI();

    // デバッグ用GUI描画
    void DrawDebugPrimitive();

private:

    // スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    // 移動入力処理
    void InputMove(float elapsedTime);

    // プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

protected:
    // 着地したときに呼ばれる
    void OnLanding() override;

private:
    int jumpCount = 0;
    int jumpLimit = 2;

    Model* model = nullptr;
    float  moveSpeed = 5.0f;
    float  turnSpeed = DirectX::XMConvertToRadians(720);
    float  jumpSpeed = 20.0f;

    bool HitFlg = false;
};