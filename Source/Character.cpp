#include "Character.h"
#include "SpecialArea.h"

// �s��X�V����
void Character::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
                                const DirectX::XMFLOAT3& AdjustAngle,
                                const DirectX::XMFLOAT3& AdjustPosition)
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x + AdjustScale.x,
                                                   scale.y + AdjustScale.y,
                                                   scale.z + AdjustScale.z);
    // ��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x + AdjustAngle.x,
                                                                angle.y + AdjustAngle.y,
                                                                angle.z + AdjustAngle.z);
    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + AdjustPosition.x,
                                                       position.y + AdjustPosition.y,
                                                       position.z + AdjustPosition.z);
    // �R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �ړ�����
void Character::Move(float vx, float vz, float speed)
{
    // �ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

// ���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�����x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    float length = vx * vx + vz * vz;
    if (length < 0.00001f) return;
    length = sqrtf(length);

    // �i�s�����x�N�g����P�ʃx�N�g����
    vx /= length;
    vz /= length;

    // ���g�̉�]�l����O���������߂�
    float frontZ = cosf(angle.y);
    float frontX = sinf(angle.y);

    // ��]�p�����߂邽�ߓ�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);

    //���ϒl��-1.0�`1.0�ŕ\������Ă���A��̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0f - dot;
    if (rot > speed)
    {
        rot = speed;
    }

    // ���E������s�����߂ɓ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontZ * vx) - (frontX * vz);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }

    else
    {
        angle.y += rot;
    }
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0) return false;

    // ���G���͌��N��Ԃ�ύX���Ȃ�
    if (invincibleTimer > 0.0f) return false;

    // �_���[�W�K�p
    health -= damage;

    // ���G���Ԑݒ�
    invincibleTimer = invincibleTime;

    // ���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
    }

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}

// �Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // ���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// �W�����v����
void Character::Jump(float speed)
{
    // ������̗͂�ݒ�
    velocity.y = speed;
}

// ���͏����X�V
void Character::UpdateVelocity(float elapsedTime)
{
    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    AreaManager::Instance().CollisionAreaVsCharacter(this, elapsedFrame);

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);

#if 0
    // �d�͏���
    velocity.y += gravity * elapsedFrame;

    // �ړ�����
    position.y += velocity.y * elapsedTime;

    // �n�ʔ���
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        velocity.y = 0.0f;

        // ���n����
        if (!isGround)
        {
            OnLanding();
        }
        isGround = true;
    }
    else
    {
        // �n�ʂɐڒn���Ă��Ȃ�
        isGround = false;
    }

#endif // 0
}

// �������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    // �d�͏���
    velocity.y += gravity * elapsedFrame;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
    // �ړ�����
    position.y += velocity.y * elapsedTime;

    // �ړ�����
    position.y += velocity.y * elapsedTime;

    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        velocity.y = 0.0f;
    }
}

// �������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZ���ʂ̑��͂���������
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    // �R�͂ɂ�鉡�����̌�������
    if (length > 0.0f)
    {
        // ���C��
        float friction = this->friction * elapsedFrame;

        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround) friction *= airControl;
        
        // ���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            // �P�ʃx�N�g����
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            // velocity.x = (length - friction) * vx;
            // velocity.z = (length - friction) * vz;
            //            =  length * vz;
            //            = (length - friction) * vz;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    // XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // ���������̃^�C�~���O���炵
            moveVecLength = sqrtf(moveVecLength);

            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͉����͂����炷
            if (!isGround) acceleration *= airControl;

            // �ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // �ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }

    // �ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime) 
{
    // �ړ�����
    position.x += velocity.x * elapsedTime;
    position.z += velocity.z * elapsedTime;
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
