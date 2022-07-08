#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"

class CanonBall
{
public:
     CanonBall() {}
    ~CanonBall() {}

    // �X�V����
    void Update(float elapsedTime);

    // ���x�X�V����
    void UpdateVelocity(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �j��
    void Destroy();

    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ���x�擾
    const DirectX::XMFLOAT3& GetVelocity() const { return velocity; }

    // ���x�ݒ�
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }

    // ���a�擾
    float GetRadius() const { return radius; }

    // ���a�ݒ�
    void SetRadius(const float& radius) { this->radius = radius; }

    // �����擾
    float GetHeight() const { return height; }

    // �����ݒ�
    void SetHeight(const float& height) { this->height = height; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 velocity = {};

    float radius = 0.25f;
    float height = 0.5f;

    // �e���Ŏ���
    int removeTimer = 120;
};