#pragma once
/*
* Log:
* Fuji {
*	#include:	BaseScene
*	SceneGame:	Manager�ŊǗ����邽�߂Ɋ��N���X��ݒ�B
*  } - 2022�N6��29��15:23
* Fuji {
*	SceneGame:	getType()��`��.cpp�Ɉړ�
*	#include:	SceneManager(.cpp)
* } - 2022�N7��1��9:14
*/


#include "BaseScene.h"

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	/* instance getter */
	static SceneGame& Instance() {
		static SceneGame instance;
		return instance;
	}

	//------------------------//
private:
	// constructer etc.
	SceneGame() {}
	~SceneGame()override {}

	/* main method */
	void Initialize()override;				// ������
	void Finalize()override;				// �I����
	void Update(float elapsedTime)override;	// �X�V����
	void Render()override;					// �`�揈��

	/* get type */
	const SCENE_TYPE getType();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
};

