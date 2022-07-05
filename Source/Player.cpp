#include <imgui.h>

#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "HoleManager.h"
#include "CanonManager.h"
#include "CanonBallManager.h"

#include "Input/Input.h"
#include "Graphics//Graphics.h"

// �R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.005f;

    radius = 0.5f;
    height = 1.0f;

    // �X�e�[�W�̒[
    position.x = position.z = -22.0f;
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

    // �傫���ύX���͏���
    InputScaleChange();

    // ���͍X�V����
    UpdateVelocity(elapsedTime);

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˋ���
    CollisionPlayerVsEnemies();

    // �v���C���[�ƌ��Ƃ̏Փˋ���
    CollisionPlayerVsHoles();

    // �v���C���[�ƖC��Ƃ̏Փˋ���
    CollisionPlayerVsCanons();

    // �v���C���[�ƒe�Ƃ̏Փˋ���
    CollisionPlayerVsCanonBalls();

    // �I�u�W�F�N�g�s����X�V
    UpdateTransform(DirectX::XMFLOAT3(ScaleNum, ScaleNum, ScaleNum),
                    DirectX::XMFLOAT3(0, 0, 0),
                    DirectX::XMFLOAT3(0, PositionNum, 0));

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
            // �ړ����x
            ImGui::InputFloat3("Velocity", &velocity.x);
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
    // ������p�̉~��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
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

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˋ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        // �Փˏ���
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
            // �����o����̈ʒu�ݒ�
            SetPosition(outPosition);

            HitFlg = true;
        }
        else HitFlg = false;
    }
}

// �傫���ύX���͏���
void Player::InputScaleChange()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_A) ScaleFlg = !ScaleFlg;

    // �������T�C�Y
    if (!ScaleFlg)
    {
        radius = 0.5f;
        height = 1.0f;

        ScaleNum = 0.0f;
        PositionNum = height * 0.5f;
    }
    // �傫���T�C�Y
    else if (ScaleFlg)
    {
        radius = 1.0f;
        height = 1.5f;

        ScaleNum = 0.005f * 1.0f;
        PositionNum = height * 0.5f;
    }
}

// �v���C���[�ƌ��Ƃ̏Փˋ���
void Player::CollisionPlayerVsHoles()
{
    HoleManager& holeManager = HoleManager::Instance();

    // �S�Ă̌��Ƒ�������ŏՓˏ���
    int holeCount = holeManager.GetHoleCount();
    for (int i = 0; i < holeCount; ++i)
    {
        Hole* hole = holeManager.GetHole(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder
        (position,
            radius,
            height,
            hole->GetPosition(),
            hole->GetRadius(),
            hole->GetHeight(),
            outPosition))
        {
            // �v���C���[�̔��a�ƌ��̔��a���ׂ�
            if (radius <= hole->GetRadius())
            {
                float xSpeed = 0.0f;
                float zSpeed = 0.0f;

                // ���S�Ɍ������ė����Ă���
                // X���W
                if (position.x < hole->GetPosition().x)
                {
                    xSpeed = fallSpeed;
                }
                else if (position.x > hole->GetPosition().x)
                {
                    xSpeed = -fallSpeed;
                }
                else if (position.x == hole->GetPosition().x)
                {
                    xSpeed = 0.0f;
                }
                // Z���W
                if (position.z < hole->GetPosition().z)
                {
                    zSpeed = fallSpeed;
                }
                else if (position.z > hole->GetPosition().z)
                {
                    zSpeed = -fallSpeed;
                }
                else if (position.z == hole->GetPosition().z)
                {
                    zSpeed = 0.0f;
                }

                position.x += xSpeed;
                position.z += zSpeed;

                FallStartFlg = true;

                if (Collision::IntersectCylinderVsCylinder(
                    position,
                    radius,
                    height,
                    hole->GetPosition(),
                    hole->GetRadius() * 0.75f,
                    hole->GetHeight(),
                    outPosition))
                {
                    FallStartFlg = false;
                    FallFlg = true;
                }
                if (!ScaleFlg && hole->GetRadius() >= 4.0f)
                {
                    FallStartFlg = false;
                    FallFlg = true;
                }
            }
            else
            {
                FallStartFlg = false;
            }
        }
        else
        {
            FallStartFlg = false;
        }
    }
}

// �v���C���[�ƖC��Ƃ̏Փˋ���
void Player::CollisionPlayerVsCanons()
{
    CanonManager& canonManager = CanonManager::Instance();

    // �S�Ă̖C��Ƒ�������ŏՓˏ���
    int canonCount = canonManager.GetCanonCount();
    for (int i = 0; i < canonCount; ++i)
    {
        Canon* canon = canonManager.GetCanon(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectBoxVsBox_Wall
        (position,
            1.0f,
            1.0f,
            1.0f,
            canon->GetPosition(),
            canon->GetLength().x,
            canon->GetLength().y,
            canon->GetLength().z,
            outPosition))
        {
            // �v���C���[�������o��
            SetPosition(outPosition);
        }
    }
}

// �v���C���[�ƒe�Ƃ̏Փˋ���
void Player::CollisionPlayerVsCanonBalls()
{
    CanonBallManager& canonBallManager = CanonBallManager::Instance();

    // �S�Ă̒e�Ƒ�������ŏՓˏ���
    int canonBallCount = canonBallManager.GetCanonBallCount();
    for (int i = 0; i < canonBallCount; ++i)
    {
        CanonBall* canonBall = canonBallManager.GetCanonBall(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder
        (position,
            radius,
            height,
            canonBall->GetPosition(),
            canonBall->GetRadius(),
            canonBall->GetHeight(),
            outPosition))
        {
            // �v���C���[�������o��
            {
                DirectX::XMFLOAT3 impulse;
                const float power = 4.0f;
                DirectX::XMFLOAT3 p = position;
                DirectX::XMFLOAT3 b = canonBall->GetPosition();

                float vx = p.x - b.x;
                float vz = p.z - b.z;
                float len = sqrtf(vx * vx + vz * vz);

                vx /= len;
                vz /= len;

                impulse.x = vx * power;
                impulse.z = vz * power;

                AddImpulse(impulse);
            }
        }
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}
    