#pragma once

#include <DirectXMath.h>

class Hole
{
public:
     Hole() { height = 1.5f; }
    ~Hole() {}

    // �X�V����
    void Update(float elapsedTime);

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �j��
    void Destroy();

    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ���a�擾
    float GetRadius() const { return radius; }

    // ���a�ݒ�
    void SetRadius(const float& radius) { this->radius = radius; }

    // �����擾
    float GetHeight() const { return height; }

    // �����ݒ�
    void SetHeight(const float& height) { this->height = height; }

private:
    DirectX::XMFLOAT3 position = {};

    float radius = 1.0f;
    float height = 3.0f;
};