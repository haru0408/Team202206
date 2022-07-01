#pragma once
/*
* Log:
* Fuji {
*	#include:	BaseScene
*	SceneGame:	Managerで管理するために基底クラスを設定。
*  } - 2022年6月29日15:23
* Fuji {
*	SceneGame:	getType()定義を.cppに移動
*	#include:	SceneManager(.cpp)
* } - 2022年7月1日9:14
*/


#include "BaseScene.h"

#include "Stage.h"
#include "Player.h"
#include "CameraController.h"

// ゲームシーン
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
	void Initialize()override;				// 初期化
	void Finalize()override;				// 終了化
	void Update(float elapsedTime)override;	// 更新処理
	void Render()override;					// 描画処理

	/* get type */
	const SCENE_TYPE getType();

private:
	Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
};

