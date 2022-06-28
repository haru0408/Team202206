#include <imgui.h>
#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Input/Input.h"
#include "Graphics//Graphics.h"

// �R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;
}

Player::~Player()
{
    delete model;
}

// �X�V����
void Player::Update(float elapsedTime)
{
    // �ړ����͏���
    InputMove(elapsedTime);

    // ���͍X�V����
    UpdateVelocity(elapsedTime);

    // �e�ۍX�V����
    projectileManager.Update(elapsedTime);

    // �I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

// �ړ����͏���
void Player::InputMove(float elapsedTime)
{
    // �i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    // ���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// �`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    // �e�ە`�揈��
    projectileManager.Render(dc, shader);
}

// �f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);



    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // �ʒu
            ImGui::InputFloat3("Position", &position.x);
            // ��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            ImGui::SliderFloat3("Angle", &a.x, 0, 360);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            // �X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);
        }

    }
    ImGui::End();

    // �`���[�g���A��
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
    // �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
#endif // 0

    // �Փ˔���p�̃f�o�b�O�~����`��
    if (HitFlg) debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
    else if (!HitFlg) debugRenderer->DrawBox(position, 1.0f, 1.0f, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));
    // �e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // ���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX      = cameraRight.x;
    float cameraRightZ      = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX      = cameraFront.x;
    float cameraFrontZ      = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //
    // vec =  (ax * cameraRight + ay * cameraFront);

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ax * cameraRightZ + ay * cameraFrontZ;
    // Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

//void Player::CollisionProjectilesVsEnemies()
//{
//    EnemyManager& enemyManager = EnemyManager::Instance();
//
//    // �S�Ă̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓˏ���
//    int projectileCount = projectileManager.GetProjectileCount();
//    int enemyCount = enemyManager.GetEnemyCount();
//    for(int i = 0; i < projectileCount; ++i)
//    {
//        Projectile* projectile = projectileManager.GetProjectile(i);
//
//        for (int j = 0; j < enemyCount; ++j)
//        {
//            Enemy* enemy = enemyManager.GetEnemy(j);
//
//            // �Փˏ���
//            DirectX::XMFLOAT3 outPosition;
//            if (Collision::IntersectSphereVsCylinder(
//                projectile->GetPosition(),
//                projectile->GetRadius(),
//                enemy->GetPosition(),
//                enemy->GetRadius(),
//                enemy->GetHeight(),
//                outPosition))
//            {
//                // �_���[�W��^����
//                if (enemy->ApplyDamage(1, 0.5f))
//                {
//                    // ������΂�
//                    {
//                        DirectX::XMFLOAT3 impulse;
//                        const float power = 10.0f;
//
//                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
//                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
//                        
//                        float vx = e.x - p.x;
//                        float vz = e.z - p.z;
//                        float len = sqrtf(vx * vx + vz * vz);
//                        vx /= len;
//                        vz /= len;
//
//                        impulse.x = vx * power;
//                        impulse.z = vz * power;
//                        impulse.y = power * 0.5f;
//
//                        enemy->AddImpulse(impulse);
//                    }
//
//                    // �e�۔j��
//                    projectile->Destroy();
//                }
//            }
//        }
//    }
//}

void Player::OnLanding()
{
    jumpCount = 0;
}
    