#include "SceneGame.h"
#include "SceneManager.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"

#include "Camera.h"

#include "EnemySlime.h"
#include "EnemyManager.h"

#include "SpecialArea.h"
#include "HoleManager.h"
#include "CanonManager.h"
#include "CanonBallManager.h"
#include "SpringManager.h"

#include "NormalFloor.h"
#include "DisappearingFloor.h"
#include "AccelerationFloor.h"
#include "FloorManager.h"

#include "EffectManager.h"

// 初期化
void SceneGame::Initialize()
{
	// ステージ初期化
	stage = new Stage();

	// プレイヤー初期化
	player = new Player();

	// カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	// カメラコントローラー初期化
	cameraController = new CameraController;

	//床の初期化
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			for (int y = 0; y < 6; y++)
			{
				switch (y)
				{
				case 0:
					SetMap(Floor_0, x, z, y);
					break;
				case 1:
					SetMap(Floor_1, x, z, y);
					break;
				case 2:
					SetMap(Floor_2, x, z, y);
					break;
				case 3:
					SetMap(Floor_3, x, z, y);
					break;
				case 4:
					SetMap(Floor_4, x, z, y);
					break;
				case 5:
					SetMap(Floor_5, x, z, y);
					break;
				}
			}
		}
	}

	// 風エリアの初期化
	AreaManager::Instance().Register(new AreaWindow(DirectX::XMFLOAT3(13.75f, 100.0f, 2.3f), 
		DirectX::XMFLOAT3(11.0f, 2.0f, 3.0f), 
		DirectX::XMFLOAT3(-0.4f, 0.0f, 0.0f)));
	wind1Effect = new Effect("Data/Effect/WindLeftEffect.efk");
	AreaManager::Instance().Register(new AreaWindow(DirectX::XMFLOAT3(-0.5f, 100.0f, 10.0f),
		DirectX::XMFLOAT3(3.0f, 2.0f, 5.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, -0.4f)));
	wind2Effect = new Effect("Data/Effect/WindDownEffect.efk");
	AreaManager::Instance().Register(new AreaWindow(DirectX::XMFLOAT3(-9.0f, 100.0f, 16.5f),
		DirectX::XMFLOAT3(3.0f, 2.0f, 4.5f),
		DirectX::XMFLOAT3(0.0f, 0.0f, -0.4)));
	wind3Effect = new Effect("Data/Effect/WindDownEffect_.efk");

	warpEffect = new Effect("Data/Effect/warp.efk");

	// 穴の初期化
	HoleInitialize();

	// 砲台の初期化
	//CanonInitialize();

	SpringInitialize();
}

// 終了化
void SceneGame::Finalize()
{
	// プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	// ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	// エネミー終了化
	EnemyManager::Instance().Clear();

	FloorManager::Instance().Clear();

	AreaManager::Instance().Clear();

	// 穴終了化
	HoleManager::Instance().Clear();

	// 砲台終了化
	CanonManager::Instance().Clear();
	// 弾終了化
	CanonBallManager::Instance().Clear();

	//バネ終了化
	SpringManager::Instance().Clear();

	// カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	// エフェクト終了化
	delete wind1Effect;
	delete wind2Effect;
	delete wind3Effect;
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	// ステージ更新処理
	stage->Update(elapsedTime);
	// プレイヤー更新処理
	player->Update(elapsedTime);
	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	FloorManager::Instance().Update(elapsedTime);

	// 砲台更新処理
	CanonManager::Instance().Update(elapsedTime);
	// 弾更新処理
	CanonBallManager::Instance().Update(elapsedTime);

	//バネの更新処理
	SpringManager::Instance().Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	// エフェクト再生
	wind1Effect->Play(DirectX::XMFLOAT3(16.75f, 101.0f, 2.3f));
	wind2Effect->Play(DirectX::XMFLOAT3(-0.5f, 101.0f, 12.5f));
	wind2Effect->Play(DirectX::XMFLOAT3(-9.0f, 100.0f, 16.5f));
	if ((Timer % 120) == 0) warpEffect->Play(DirectX::XMFLOAT3(0.0f, 20.0f, 0.0f));

	++Timer;

	// change scene
	//if (Input::Instance().GetGamePad().GetButtonDown()
	//	& GamePad::BTN_BACK)
	//	SceneManager::Instance().changeScene(SCENE_TYPE::TITLE);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

#if 0
	// ビュー行列
	{
		DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
		DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
		DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向

		DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
		DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
		DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
		DirectX::XMStoreFloat4x4(&rc.view, View);
	}
	// プロジェクション行列
	{
		float fovY = DirectX::XMConvertToRadians(45);	// 視野角
		float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
		float nearZ = 0.1f;	// カメラが映し出すの最近距離
		float farZ = 1000.0f;	// カメラが映し出すの最遠距離
		DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
		DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	}
#endif
	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		// ステージ描画
		stage->Render(dc, shader);

		// プレイヤー描画
		player->Render(dc, shader);

		// プレイヤー描画
		EnemyManager::Instance().Render(dc, shader);

		FloorManager::Instance().Render(dc, shader);

		// 砲台描画
        //CanonManager::Instance().Render(dc, shader);
        // 弾描画
        //CanonBallManager::Instance().Render(dc, shader);

		SpringManager::Instance().Render(dc, shader);

		shader->End(dc);
	}
	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		// プレーヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		// エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		//AreaManager::Instance().DrawDebugPrimitive();

		//FloorManager::Instance().DrawDebugPrimitive();

		// 穴デバッグプリミティブ描画
		//HoleManager::Instance().DrawDebugPrimitive();

		// 砲台デバッグプリミティブ描画
		//CanonManager::Instance().DrawDebugPrimitive();
		// 弾デバッグプリミティブ描画
		//CanonBallManager::Instance().DrawDebugPrimitive();

		SpringManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{

	}

	// 2DデバッグGUI描画
	{
		//　プレイヤーデバッグ描画
		player->DrawDebugGUI();
		//　カメラコントローラーデバッグ描画
		cameraController->DrawDebugGUI();

		//　ImGui描画
		HoleManager::Instance().DrawDebugGUI();

		CanonManager::Instance().DrawDebugGUI();
	}
}

void SceneGame::SetMap(int Floor[20][20], int x, int z, int y)
{
	//床の配置
	FloorManager& floorManager = FloorManager::Instance();

	if (Floor[x][z] == 0)
	{
		NormalFloor* Normalfloor = new NormalFloor();
		Normalfloor->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Normalfloor->SetFloorStage(y);
		floorManager.Register(Normalfloor);
	}

	if (Floor[x][z] == 1)
	{
		DisappearingFloor* Disapperaringfloor = new DisappearingFloor();
		Disapperaringfloor->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Disapperaringfloor->SetFloorStage(y);
		floorManager.Register(Disapperaringfloor);
	}

	if (Floor[x][z] == 2)
	{
		AccelerationFloor* Accelerationfloor_Left = new AccelerationFloor(0);
		Accelerationfloor_Left->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Accelerationfloor_Left->SetFloorStage(y);
		floorManager.Register(Accelerationfloor_Left);
	}

	if (Floor[x][z] == 3)
	{
		AccelerationFloor* Accelerationfloor_Down = new AccelerationFloor(1);
		Accelerationfloor_Down->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Accelerationfloor_Down->SetFloorStage(y);
		floorManager.Register(Accelerationfloor_Down);
	}

	if (Floor[x][z] == 4)
	{
		AccelerationFloor* Accelerationfloor_Right = new AccelerationFloor(2);
		Accelerationfloor_Right->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Accelerationfloor_Right->SetFloorStage(y);
		floorManager.Register(Accelerationfloor_Right);
	}

	if (Floor[x][z] == 5)
	{
		AccelerationFloor* Accelerationfloor_Up = new AccelerationFloor(3);
		Accelerationfloor_Up->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Accelerationfloor_Up->SetFloorStage(y);
		floorManager.Register(Accelerationfloor_Up);
	}

}
/* get type */
const SCENE_TYPE SceneGame::getType() { return SCENE_TYPE::GAME; }

// 穴の初期化
void SceneGame::HoleInitialize()
{
	// インスタンス取得
	HoleManager& holeManager = HoleManager::Instance();

	// Stage01
	Hole* hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-6.5f, 20.0f, 11.3f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-12.9f, 20.0f, -0.25f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-6.1f, 20.0f, -11.5f));
	hole->SetRadius(1.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(3.0f, 20.0f, -12.5f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(12.5f, 20.0f, -1.5f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(10.0f, 20.0f, 13.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, 0.0f));
	hole->SetRadius(6.25f);
	holeManager.Register(hole);

	// Stage02
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-11.8f, 40.0f, -8.6f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(6.1f, 40.0f, -14.5f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(6.1f, 40.0f, 11.3f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-10.25f, 40.0f, 3.0f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-13.5, 40.0f, 11.3f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(1.0f, 40.0f, 16.0f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(16.2f, 40.0f, 3.0f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(13.5f, 40.0f, -11.25f));
	hole->SetRadius(1.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-3.0f, 40.0f, -16.5f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 40.0f, 0.0f));
	hole->SetRadius(6.5f);
	holeManager.Register(hole);

	// Stage03
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-15.0f, 60.0f, -12.5f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-0.95f, 60.0f, -16.5f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(15.25f, 60.0f, -13.0f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(8.15f, 60.0f, -7.25f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(13.5f, 60.0f, -3.75f));
	hole->SetRadius(1.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(8.15f, 60.0f, 5.25f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(11.75f, 60.0f, 11.5f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(6.3f, 60.0f, 16.5f));
	hole->SetRadius(1.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.85f, 60.0f, 11.0f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-6.9f, 60.0f, 12.35f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-9.0f, 60.0f, 11.35f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-13.65f, 60.0f, 4.85f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-10.3f, 60.0f, 3.25f));
	hole->SetRadius(1.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.5f, 60.0f, 0.0f));
	hole->SetRadius(6.5f);
	holeManager.Register(hole);

	// Stage04
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-17.4f, 80.0f, -5.25f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-12.35f, 80.0f, 3.25f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-7.0f, 80.0f, 13.25f));
	hole->SetRadius(1.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(12.6f, 80.0f, 11.85f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(7.0f, 80.0f, 9.15f));
	hole->SetRadius(3.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(15.8f, 80.0f, 5.8f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(12.0f, 80.0f, -7.15f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-2.75f, 80.0f, -13.9f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-7.0f, 80.0f, -9.5f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.125f, 80.0f, 0.31f));
	hole->SetRadius(6.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(17.2f, 80.0f, -14.35f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(4.25f, 80.0f, -17.35f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);

	// Stage05
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-13.9f, 100.0f, -11.5f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-3.65f, 100.0f, -9.1f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(6.2f, 100.0f, -7.9f));
	hole->SetRadius(1.75f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(16.0f, 100.0f, -7.5f));
	hole->SetRadius(1.75f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(11.75f, 100.0f, -3.15f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(14.15f, 100.0f, 1.95f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(7.35f, 100.0f, 4.3f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(17.25f, 100.0f, 7.65f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-6.9f, 100.0f, 12.4f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-12.75f, 100.0f, 15.25f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.125f, 100.0f, 0.31f));
	hole->SetRadius(6.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-6.75f, 100.0f, -18.0f));
	hole->SetRadius(1.25f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-9.4f, 100.0f, -3.65f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-13.9f, 100.0f, -2.1f));
	hole->SetRadius(0.5f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(-12.4f, 100.0f, 3.1f));
	hole->SetRadius(2.75f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(6.8f, 100.0f, 9.1f));
	hole->SetRadius(2.75f);
	holeManager.Register(hole);
}

void SceneGame::CanonInitialize()
{
	// インスタンス取得
	CanonManager& canonManager = CanonManager::Instance();

	// Stage03
	Canon* canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(8.5f, 60.25f, 1.0f));
	canon->SetUpDirection();
	canonManager.Register(canon);
	canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(0.0f, 60.25f, -8.0f));
	canon->SetDownDirection();
	canonManager.Register(canon);
	canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(-11.5f, 60.25f, 0.0f));
	canon->SetRightDirection();
	canonManager.Register(canon);
	canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(-3.25f, 60.25f, 15.5f));
	canon->SetDownDirection();
	canonManager.Register(canon);
	canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(-6.5f, 60.25f, 17.75f));
	canon->SetDownDirection();
	canonManager.Register(canon);

	// Stage04
	//canon = new Canon();
	//canon->SetPosition(DirectX::XMFLOAT3(10.5f, 80.2f, 18.0f));
	//canon->SetDownDirection();
	//canonManager.Register(canon);
}

void SceneGame::SpringInitialize()
{
	// インスタンス取得
	SpringManager& springManager = SpringManager::Instance();

	// Stage01
	Spring* spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(18.5f, 20.35f, 9.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(2.1f, 20.35f, 17.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(3.75f, 20.35f, 9.4f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(9.15f, 20.35f, -6.25f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(9.15f, 20.35f, -16.75f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-8.15f, 20.35f, 11.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-9.65f, 20.35f, -12.65f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-15.6f, 20.35f, 4.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-18.5f, 20.35f, -4.25f));
	springManager.Register(spring);

	// Stage02
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(18.5f, 40.3f, 8.92f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(9.3f, 40.3f, 17.75f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-9.75f, 40.3f, 17.75f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-18.5f, 40.3f, -5.8f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-8.8f, 40.3f, -18.5f));
	springManager.Register(spring);

	// Stage03
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.5f, 60.25f, 17.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(11.75f, 60.25f, -16.85f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-7.45f, 60.25f, -18.5f));
	springManager.Register(spring);

	// Stage04
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.5f, 80.2f, 17.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(18.5f, 80.2f, -9.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-18.5f, 80.2f, 3.75f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(2.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-7.8f, 80.2f, -19.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(2.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(9.35f, 80.2f, 13.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.0f + 0.5f, 80.2f, -13.0f - 1.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(2.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.0f, 80.2f, -13.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(15.0f + 0.5f, 80.2f, 15.0f - 1.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(2.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(15.0f, 80.2f, 15.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-4.5f, 80.2f, -12.75f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-11.5f, 80.2f, 15.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-12.5f, 80.2f, 18.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-4.6f, 80.2f, 18.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-3.6f, 80.2f, 15.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-2.6f, 80.2f, 13.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.35f, 80.2f, 8.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.35f, 80.2f, 10.5f));
	springManager.Register(spring);

	// Stage05
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(18.5f, 100.15f, 11.3f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(2.35f, 100.15f, 16.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-6.0f, 100.15f, 16.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(2.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-6.0f + 2.5f, 100.15f, 16.0f + 1.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-18.5f, 100.15f, 5.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-10.5f, 100.15f, -11.1f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(3.0f, 1.0f, 3.0f));
	spring->SetPosition(DirectX::XMFLOAT3(1.95f, 100.15f, -16.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(14.65f, 100.15f, 14.85f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(12.85f, 100.15f, 10.7f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(11.85f, 100.15f, 7.7f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(14.9f, 100.15f, -10.95f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(15.65f, 100.15f, -14.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(10.65f, 100.15f, -14.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(10.0f, 100.15f, -11.4f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(0.0f, 100.15f, -10.25f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-6.5f, 100.15f, -8.13f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-18.4f, 100.15f, 11.35f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-13.9f, 100.15f, 17.5f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(-8.6f, 100.15f, 10.0f));
	springManager.Register(spring);
	spring = new Spring(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	spring->SetPosition(DirectX::XMFLOAT3(2.9f, 100.15f, -8.75f));
	springManager.Register(spring);
}