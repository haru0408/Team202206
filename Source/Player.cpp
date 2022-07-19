#include <imgui.h>

#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "HoleManager.h"
#include "CanonManager.h"
#include "CanonBallManager.h"
#include "FloorManager.h"
#include "SpringManager.h"

#include "Input/Input.h"
#include "Graphics//Graphics.h"

// �R���X�g���N�^
Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.005f;

    // �v���C���[�̓����蔻��̑傫���ݒ�
    length = { 1.0f, 1.0f, 1.0f };
    radius = 0.5f;
    height = 1.0f;

    // �ʒu�̓X�e�[�W�̒[
    position.x = position.z = 17.0f;
    position.y = 100.0f;
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

    // �v���C���[�ƌ��Ƃ̏Փˋ���
    CollisionPlayerVsHoles();

    // �v���C���[�ƖC��Ƃ̏Փˋ���
    CollisionPlayerVsCanons();

    // �v���C���[�ƒe�Ƃ̏Փˋ���
    CollisionPlayerVsCanonBalls();

    //�v���C���[�Ə��̏Փ˔���
    CollisionPlayerVsFloor();

    //�v���C���[�ƃo�l�̏Փ˔���
    CollisionPlayerVsSpring();

    // �I�u�W�F�N�g�s����X�V
    UpdateTransform(DirectX::XMFLOAT3(ScaleNum, ScaleNum, ScaleNum),
                    DirectX::XMFLOAT3(0, 0, 0),
                    DirectX::XMFLOAT3(0, PositionNum, 0));

    // �{�[��(���f��)��]����
    BallRoll(elapsedTime);

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

            // 
            ImGui::InputFloat3("Length", &length.x);
        }

    }
    ImGui::End();


    ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // �`���[�g���A��
    if (ImGui::Begin("HOWTO",nullptr, ImGuiWindowFlags_None))
    {
        ImGui::Text("MOVE");
        ImGui::Text("UP    W");
        ImGui::Text("LEFT  A");
        ImGui::Text("DOWN  S");
        ImGui::Text("RIGHT D");
        ImGui::Text("");
        ImGui::Text("SizeChange : Space");
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

    // �Փ˔���p�̃f�o�b�O�v���~�e�B�u��`��
    // �l�p
    debugRenderer->DrawBox(position, length.x, length.y, length.z, DirectX::XMFLOAT4(1, 0, 0, 1));
    // �~��
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

// �{�[��(���f��)��]����
void Player::BallRoll(float elapsedTime)
{
    // �X�e�B�b�N�E�{�^�����͒l�ɂ���ă{�[��(���f��)��]����
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    const float RollSpeed = 0.001f;

    // �΂߈ړ�
    if (ax != 0.0f && ay != 0.0f)
    {
        ax *= ay;

        if (ax > 0.0f) angle.x += ax / elapsedTime * (RollSpeed * 2.0f);
        else if (ax < 0.0f) angle.x -= ax / elapsedTime * (RollSpeed * 2.0f);
    }
    // ���ړ�
    else if (ax != 0.0f)
    {
        if (ax > 0.0f) angle.x += ax / elapsedTime * RollSpeed;
        else if (ax < 0.0f) angle.x -= ax / elapsedTime * RollSpeed;
    }
    // �c�ړ�
    else if (ay != 0.0f)
    {
        if (ay > 0.0f) angle.x += ay / elapsedTime * RollSpeed;
        else if (ay < 0.0f) angle.x -= ay / elapsedTime * RollSpeed;
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
        length = { 1.0f, 1.0f, 1.0f };

        radius = 0.5f;
        height = 1.0f;

        ScaleNum = 0.0f;
        PositionNum = height * 0.5f;

        if      (position.x > 19.5f) position.x  = 19.5f;
        else if (position.x < -19.5f) position.x = -19.5f;

        if      (position.z > 19.5f)  position.z = 19.5f;
        else if (position.z < -19.5f) position.z = -19.5f;
    }
    // �傫���T�C�Y
    else if (ScaleFlg)
    {
        length = { 2.0f, 2.0f, 2.0f };

        radius = 1.0f;
        height = 1.5f;

        ScaleNum = 0.005f * 1.0f;
        PositionNum = height * 0.65f;

        if       (position.x > 19.0f) position.x = 19.0f;
        else if (position.x < -19.0f) position.x = -19.0f;

        if       (position.z > 19.0f)  position.z = 19.0f;
        else if (position.z < -19.0f) position.z = -19.0f;

        velocity.y += 1;
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

        // �Փˏ���(�~��)
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            position,
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

                if (Collision::IntersectCylinderVsCylinder(
                    position,
                    radius,
                    height,
                    hole->GetPosition(),
                    hole->GetRadius() * 0.75f,
                    hole->GetHeight(),
                    outPosition))
                {
                    fallFlg = true;
                    holePosY = hole->GetPosition().y - 5.0f;
                }
                // �v���C���[�̔��a�ƌ��̔��a�傫�ȍ�������ΊO���̉~���ɏՓ˂������_�ŗ���
                if (!ScaleFlg && hole->GetRadius() >= 4.0f)
                {
                    fallFlg = true;
                    holePosY = hole->GetPosition().y - 5.0f;
                }
            }
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

        // �Փˏ���(�l�p)
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectBoxVsBox_Wall(
            position,
            length.x,
            length.y,
            length.z,
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

    // �S�Ă̒e�Ƒ�������ŏՓˏ���(�~��)
    int canonBallCount = canonBallManager.GetCanonBallCount();
    for (int i = 0; i < canonBallCount; ++i)
    {
        CanonBall* canonBall = canonBallManager.GetCanonBall(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            position,
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

void Player::CollisionPlayerVsFloor()
{
    FloorManager& floorManager = FloorManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int floorCount = floorManager.GetFloorCount();
    for (int i = 0; i < floorCount; ++i)
    {
        Floor* floor = floorManager.GetFloor(i);

        if (velocity.y <= 0.0f)
        {
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectBoxVsBox_Ground(
                position,
                length.x,
                length.y,
                length.z,
                floor->GetPosition(),
                2.0f,
                1.0f,
                2.0f,
                outPosition))
            {
                if (!fallFlg)
                {
                    // �����o����̈ʒu�ݒ�
                    SetPosition(outPosition);
                    velocity.y = 0.0f;
                }
                else if (fallFlg)
                {
                    if (holePosY > floor->GetPosition().y)
                    {
                        SetPosition(outPosition);
                        velocity.y = 0.0f;
                        fallFlg = false;
                    }
                }

                if (floor->GetFloorNum() == 1)
                {
                    velocity = floor->SetImpulse(floor->floor_direction);
                    ScaleFlg = false;
                }
            }
            if (Collision::IntersectBoxVsBox_Ground(
                DirectX::XMFLOAT3(position.x, position.y - 0.01f, position.z),
                length.x,
                length.y,
                length.z,
                floor->GetPosition(),
                3.0f,
                1.0f,
                3.0f,
                outPosition) &&
                floor->GetFloorNum() == 0)
            {
                floor->Destroy_timer();
            }
        }
            
        
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}
    
void Player::Revelo()
{
    if (velocity.x + velocity.z < 10)
    {
        velocity.x *= -3.0f;
        velocity.z *= -3.0f;
    }
    else
    {
        velocity.x *= -1.0f;
        velocity.z *= -1.0f;
    }


}

void Player::CollisionPlayerVsSpring()
{
    SpringManager& springManager = SpringManager::Instance();

    int SpringCount = springManager.GetSpringCount();

    // �Փˏ���
    DirectX::XMFLOAT3 outPosition;
    for (int i = 0; i < SpringCount; i++)
    {
        Spring* spring = springManager.GetSpring(i);
        if (Collision::IntersectBoxVsBox_Wall(
            position,
            length.x,
            length.y,
            length.z,
            spring->GetPosition(),
            spring->GetLength().x,
            spring->GetLength().y,
            spring->GetLength().z,
            outPosition))
        {
            if (velocity.x + velocity.z < 6)
            {
                Revelo();
                ScaleFlg = false;
            }
            else
            {
                velocity.x = 0;
                velocity.z = 0;
                SetPosition(outPosition);
            }
        }
          
    }
}