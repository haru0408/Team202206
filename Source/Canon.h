#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"

class Canon
{
public:
     Canon() {}
    ~Canon() {}

    // �X�V����
    void Update(float elapsedTime);

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

    // �����擾
    const DirectX::XMFLOAT3& GetLength() const { return length; }

    // �����ݒ�
    void SetLength(const DirectX::XMFLOAT3& length) { this->length = length; }

    // �����擾
    const int& GetDirection() const { return direction; }

    // ������ݒ�
    void SetUpDirection()    { this->direction = 1; }
    // �������ݒ�
    void SetDownDirection()  { this->direction = 2; }
    // �������ݒ�
    void SetLeftDirection()  { this->direction = 3; }
    // �E�����ݒ�
    void SetRightDirection() { this->direction = 4; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 length   = { 1.0f, 1.0f, 1.0f };

    // ��������p�ϐ�
    // 1:�����
    // 2:������
    // 3:������
    // 4:�E����
    int direction = 0;

    // ���ˊԊu����
    int LanchTimer = 60;
};