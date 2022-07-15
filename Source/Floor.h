#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"

class Floor
{
public:
    Floor() {}

    virtual ~Floor() {}

    virtual void UpdateTransform();

    // �X�V����
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //�f�o�b�O
    virtual void DrawDebugPrimitive();

    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    virtual void Destroy_timer() {};

    virtual DirectX::XMFLOAT3 SetImpulse(int judge)=0;

    void SetFloorStage(int set) { this->Floor_Stage = set; };

    int GetFloorStage() { return Floor_Stage; };
    // �j��
    void Destroy();

    int GetFloorNum() const { return floor_num; }

    void SetFloor_Direction(int set) { floor_direction = set; };

    int floor_direction = 0;

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

    int floor_num = 0;

    int Floor_Stage;
};
