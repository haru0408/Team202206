#pragma once
#include "Floor.h"
class DisappearingFloor : public Floor
{
public:
	DisappearingFloor();
	~DisappearingFloor()override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void Destroy_timer() override;

	DirectX::XMFLOAT3 SetImpulse(int judge)override { return null; };

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 null = { 0.0f, 0.0f, 0.0f };

	float timer = 0;
};