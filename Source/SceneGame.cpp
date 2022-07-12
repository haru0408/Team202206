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

#include "NormalFloor.h"
#include "DisappearingFloor.h"
#include "AccelerationFloor.h"
#include "FloorManager.h"

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

#if 0
	EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	enemyManager.Register(slime);
#endif

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

	AreaManager::Instance().Register(new AreaWindow(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(5, 2, 5)));

	
	// 穴の初期化
	HoleManager& holeManager = HoleManager::Instance();
	// Stage01
	Hole* hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);
	hole = new Hole();
	hole->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, -5.0f));
	hole->SetRadius(2.0f);
	holeManager.Register(hole);

	/*
	// 砲台の初期化
	CanonManager& canonManager = CanonManager::Instance();
	// Stage00
	Canon* canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, -5.0f));
	canon->SetRightDirection();
	canonManager.Register(canon);
	*/
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

	// カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
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

	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

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

		// 砲台描画
        //CanonManager::Instance().Render(dc, shader);
        // 弾描画
        //CanonBallManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		// プレーヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		// エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		AreaManager::Instance().DrawDebugPrimitive();

		FloorManager::Instance().DrawDebugPrimitive();

		// 穴デバッグプリミティブ描画
		HoleManager::Instance().DrawDebugPrimitive();

		// 砲台デバッグプリミティブ描画
		CanonManager::Instance().DrawDebugPrimitive();
		// 弾デバッグプリミティブ描画
		CanonBallManager::Instance().DrawDebugPrimitive();

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
		AccelerationFloor* Accelerationfloor = new AccelerationFloor();
		Accelerationfloor->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Accelerationfloor->SetFloorStage(y);
		floorManager.Register(Accelerationfloor);
	}

	if (Floor[x][z] == 2)
	{
		DisappearingFloor* Disapperaringfloor = new DisappearingFloor();
		Disapperaringfloor->SetPosition(DirectX::XMFLOAT3(-19.0f + (x * 2.0f), -0.6f + (y * 19.95), -19.0f + (z * 2.0f)));
		Disapperaringfloor->SetFloorStage(y);
		floorManager.Register(Disapperaringfloor);
	}

}
/* get type */
const SCENE_TYPE SceneGame::getType() { return SCENE_TYPE::GAME; }

