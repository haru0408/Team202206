#pragma once
#include "Floor.h"
class AccelerationFloor:public Floor 
{
public:
	AccelerationFloor(int judge);
	~AccelerationFloor()override;
	
	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void Destroy_timer(Floor* floor, int i) {};

	DirectX::XMFLOAT3 SetImpulse(int judge) override;
private:
	Model* model = nullptr;

	DirectX::XMFLOAT3 impulse;
};