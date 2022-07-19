#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Haruta/warp.h"

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

    //移動反転
    void Revelo();

    // スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    // 移動入力処理
    void InputMove(float elapsedTime);

    // ボール(モデル)回転処理
    void BallRoll(float elapsedTime);

    // 大きさ変更入力処理
    void InputScaleChange();

    // プレイヤーと穴との衝突距離
    void CollisionPlayerVsHoles();

    // プレイヤーと砲台との衝突距離
    void CollisionPlayerVsCanons();

    // プレイヤーと弾との衝突距離
    void CollisionPlayerVsCanonBalls();

    //プレイヤーと床との衝突判定
    void CollisionPlayerVsFloor();

    //プレイヤーとバネとの衝突判定
    void CollisionPlayerVsSpring();

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

    DirectX::XMFLOAT3 length = { 0.0f, 0.0f, 0.0f };

    DirectX::XMFLOAT3 impulse = {};

    bool  ScaleFlg    = false; // true:大 false:小
    float ScaleNum    = 0.0f;  // スケール補正値
    float PositionNum = 0.0f;  // 座標補正値

    float fallSpeed = 0.025f;
    float holePosY  = 0.0f;
    bool fallFlg    = false;
};