#pragma once
#include "Floor.h"
class NormalFloor :public Floor
{
public:
	NormalFloor();
	~NormalFloor()override;


	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void Destroy_timer(Floor* floor, int i) {};
	DirectX::XMFLOAT3 SetImpulse(int judge)override { return null; };
private:
	Model* model = nullptr;
	DirectX::XMFLOAT3 null;
};