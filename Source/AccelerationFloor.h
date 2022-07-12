#pragma once
#include "Floor.h"
class AccelerationFloor:public Floor 
{
public:
	AccelerationFloor();
	~AccelerationFloor()override;

	
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void Destroy_timer(Floor* floor, int i) {};

	DirectX::XMFLOAT3 SetImpulse()override;
private:
	Model* model = nullptr;

};