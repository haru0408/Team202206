#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"

// �v���C���[
class Player : public Character
{
public:
    Player();
    ~Player() override;

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    // �f�o�b�O�pGUI�`��
    void DrawDebugPrimitive();

private:

    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    // �ړ����͏���
    void InputMove(float elapsedTime);

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

protected:
    // ���n�����Ƃ��ɌĂ΂��
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