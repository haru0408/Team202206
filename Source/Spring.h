#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
class Spring
{
public:
	Spring(DirectX::XMFLOAT3 Length);
	~Spring();

	DirectX::XMFLOAT3 Revers_Velo();

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �s��X�V����
    void UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
        const DirectX::XMFLOAT3& AdjustAngle,
        const DirectX::XMFLOAT3& AdjustPosition);

    //�f�o�b�O
    void DrawDebugPrimitive();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // �����擾
    const DirectX::XMFLOAT3& GetLength() const { return length; }

    // �����ݒ�
    void SetLength(const DirectX::XMFLOAT3& length) { this->length = length; }

private:
    Model* model = nullptr;

    DirectX::XMFLOAT3     position = { 0, 0, 0 };
    DirectX::XMFLOAT3     angle = { 0, 0, 0 };
    DirectX::XMFLOAT3     scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4   transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    DirectX::XMFLOAT3     length = { 0.0f, 0.0f, 0.0f };
};
