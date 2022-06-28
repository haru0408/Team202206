#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "SpecialArea.h"

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

	// エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
#if 0
	EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	enemyManager.Register(slime);
#endif
	for (int i = 0; i < 2; ++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		enemyManager.Register(slime);
	}

	AreaManager::Instance().Register(new AreaWindow(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(5, 2, 5)));
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

	AreaManager::Instance().Clear();

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


	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
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

		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		// プレーヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		// エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		AreaManager::Instance().DrawDebugPrimitive();

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
	}
}
