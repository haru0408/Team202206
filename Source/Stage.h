#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"

// ステージ
class Stage
{
public:
    Stage();
    ~Stage();

    //　更新処理
    void Update(float elapsedTime);

    // 行列更新処理
    void UpdateTransform();

    // 描画処理
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