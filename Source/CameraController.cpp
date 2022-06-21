#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <imgui.h>

// 更新処理
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    // カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    // スティックの入力値に合わせてX軸とY軸を回転
    angle.x += ay * speed;
    angle.y += ax * speed;

    // カメラの回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(
                                    angle.x, 
                                    angle.y, 
                                    angle.z);

    // X軸のカメラ回転を制限
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // Y軸の回転地を-3.14～3.14に収まるようにする
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    // 回転行列から前方向をベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2]; //行列3行目のデータ
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);
    
    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("CameraController", ImGuiTreeNodeFlags_DefaultOpen))
        {
          // 位置
          ImGui::InputFloat3("Target", &target.x);
          // 回転
          DirectX::XMFLOAT3 a;
          a.x = DirectX::XMConvertToDegrees(angle.x);
          a.y = DirectX::XMConvertToDegrees(angle.y);
          a.z = DirectX::XMConvertToDegrees(angle.z);
          ImGui::InputFloat3("Angle", &a.x);
          angle.x = DirectX::XMConvertToRadians(a.x);
          angle.y = DirectX::XMConvertToRadians(a.y);
          angle.z = DirectX::XMConvertToRadians(a.z);

          float r = DirectX::XMConvertToDegrees(rollSpeed);
          float min = DirectX::XMConvertToDegrees(minAngleX);
          float max = DirectX::XMConvertToDegrees(maxAngleX);

          ImGui::InputFloat("RollSpeed", &r);
          ImGui::InputFloat("Range", &range);
          ImGui::InputFloat("MinAngleX", &min);
          ImGui::InputFloat("MaxAngleX", &max);
        }
    }
    ImGui::End();
}

