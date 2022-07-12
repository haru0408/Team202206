#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
class Spring
{
public:
	Spring();
	~Spring();

	DirectX::XMFLOAT3 Revers_Velo();

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    void UpdateTransform();

    //�f�o�b�O
    void DrawDebugPrimitive();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
private:
protected:
    DirectX::XMFLOAT3     position = { 0, 0, 0 };
    DirectX::XMFLOAT3     angle = { 0, 0, 0 };
    DirectX::XMFLOAT3     scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4   transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;
};
