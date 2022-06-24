#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <imgui.h>

// �X�V����
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x += ay * speed;
    angle.y += ax * speed;

    // �J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(
                                    angle.x, 
                                    angle.y, 
                                    angle.z);

    // X���̃J������]�𐧌�
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // Y���̉�]�n��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    // ��]�s�񂩂�O�������x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2]; //�s��3�s�ڂ̃f�[�^
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);
    
    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����̎��_�ƒ����_��ݒ�
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
          // �ʒu
          ImGui::InputFloat3("Target", &target.x);
          // ��]
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
