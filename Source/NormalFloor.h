#pragma once
#include "Floor.h"
class NormalFloor :public Floor
{
public:
	NormalFloor();
	~NormalFloor()override;


	// XVˆ—
	void Update(float elapsedTime) override;
	// •`‰æˆ—
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void Destroy_timer(Floor* floor, int i) {};

	DirectX::XMFLOAT3 SetImpulse()override { return null; };

private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 null = { 0.0f, 0.0f, 0.0f };
};