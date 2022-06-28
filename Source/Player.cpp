#include <imgui.h>
#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Input/Input.h"
#include "Graphics//Graphics.h"

// コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    // モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
}

Player::~Player()
{
    delete model;
}

// 更新処理
void Player::Update(float elapsedTime)
{
    // 移動入力処理
    InputMove(elapsedTime);

    // 速力更新処理
    UpdateVelocity(elapsedTime);

    // プレイヤーとエネミーとの衝突距離
    CollisionPlayerVsEnemies();

    // オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // 移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    // 旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// 描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);



    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // 位置
            ImGui::InputFloat3("Position", &position.x);
            // 移動速度
            ImGui::InputFloat3("Velocity", &velocity.x);
            // 回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            ImGui::SliderFloat3("Angle", &a.x, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // スケール
            ImGui::InputFloat3("Scale", &scale.x);
        }

    }
    ImGui::End();

    // チュートリアル
    if (ImGui::Begin("HOWTO",nullptr, ImGuiWindowFlags_None))
    {
        ImGui::Text("MOVE");
        ImGui::Text("UP    W");
        ImGui::Text("LEFT  A");
        ImGui::Text("DOWN  S");
        ImGui::Text("RIGHT D");
        ImGui::Text("");
        ImGui::Text("ROTATE");
        ImGui::Text("X    Z");
        ImGui::Text("Y    X");
        ImGui::Text("Y    C");
    }

    ImGui::End();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

#if 0
    // 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
#endif // 0

    // 衝突判定用のデバッグ円柱を描画
    if (HitFlg) debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
    else if (!HitFlg) debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする

    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX      = cameraRight.x;
    float cameraRightZ      = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX      = cameraFront.x;
    float cameraFrontZ      = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //
    // vec =  (ax * cameraRight + ay * cameraFront);

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ax * cameraRightZ + ay * cameraFrontZ;
    // Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

// プレイヤーとエネミーとの衝突距離
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // 衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectBoxVsBox_Wall(
            position,
            1.0f,
            1.0f,
            1.0f,
            enemy->GetPosition(),
            1.0f,
            1.0f,
            1.0f,
            outPosition))
        {
            // 押し出し後の位置設定
            SetPosition(outPosition);

            HitFlg = true;
        }
        else HitFlg = false;
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}
    