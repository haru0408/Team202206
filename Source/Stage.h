#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"

// �X�e�[�W
class Stage
{
public:
    Stage();
    ~Stage();

    //�@�X�V����
    void Update(float elapsedTime);

    // �s��X�V����
    void UpdateTransform();

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

private:
    Model* model[6] = {nullptr};

    DirectX::XMFLOAT3     position[6] = { /*0, -100, 0 */};
    DirectX::XMFLOAT3     angle = { /*0, 0, 0*/ };
    DirectX::XMFLOAT3     scale = { /*1, 1, 1*/ };
    DirectX::XMFLOAT4X4   transform[6] = {
       {1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1}
    };
};