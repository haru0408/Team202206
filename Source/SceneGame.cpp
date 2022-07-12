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

// ������
void SceneGame::Initialize()
{
	// �X�e�[�W������
	stage = new Stage();

	// �v���C���[������
	player = new Player();

	// �J���������ݒ�
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
	// �J�����R���g���[���[������
	cameraController = new CameraController;

#if 0
	EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	enemyManager.Register(slime);
#endif

	//���̏�����
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

	
	// ���̏�����
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
	// �C��̏�����
	CanonManager& canonManager = CanonManager::Instance();
	// Stage00
	Canon* canon = new Canon();
	canon->SetPosition(DirectX::XMFLOAT3(5.0f, 0.0f, -5.0f));
	canon->SetRightDirection();
	canonManager.Register(canon);
	*/
}

// �I����
void SceneGame::Finalize()
{
	// �v���C���[�I����
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	// �X�e�[�W�I����
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	FloorManager::Instance().Clear();

	AreaManager::Instance().Clear();

	// ���I����
	HoleManager::Instance().Clear();

	// �C��I����
	CanonManager::Instance().Clear();
	// �e�I����
	CanonBallManager::Instance().Clear();

	// �J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	// �X�e�[�W�X�V����
	stage->Update(elapsedTime);
	// �v���C���[�X�V����
	player->Update(elapsedTime);
	// �G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	FloorManager::Instance().Update(elapsedTime);
	// �C��X�V����
	CanonManager::Instance().Update(elapsedTime);
	// �e�X�V����
	CanonBallManager::Instance().Update(elapsedTime);

	// �J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	// change scene
	//if (Input::Instance().GetGamePad().GetButtonDown()
	//	& GamePad::BTN_BACK)
	//	SceneManager::Instance().changeScene(SCENE_TYPE::TITLE);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

#if 0
	// �r���[�s��
	{
		DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// �J�����̎��_�i�ʒu�j
		DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// �J�����̒����_�i�^�[�Q�b�g�j
		DirectX::XMFLOAT3 up = { 0, 1, 0 };		// �J�����̏����

		DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
		DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
		DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
		DirectX::XMStoreFloat4x4(&rc.view, View);
	}
	// �v���W�F�N�V�����s��
	{
		float fovY = DirectX::XMConvertToRadians(45);	// ����p
		float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// ��ʏc���䗦
		float nearZ = 0.1f;	// �J�������f���o���̍ŋߋ���
		float farZ = 1000.0f;	// �J�������f���o���̍ŉ�����
		DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
		DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	}
#endif
	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		// �X�e�[�W�`��
		stage->Render(dc, shader);

		// �v���C���[�`��
		player->Render(dc, shader);

		// �v���C���[�`��
		EnemyManager::Instance().Render(dc, shader);

		// �C��`��
        //CanonManager::Instance().Render(dc, shader);
        // �e�`��
        //CanonBallManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	// 3D�f�o�b�O�`��
	{
		// �v���[���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		// �G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		AreaManager::Instance().DrawDebugPrimitive();

		FloorManager::Instance().DrawDebugPrimitive();

		// ���f�o�b�O�v���~�e�B�u�`��
		HoleManager::Instance().DrawDebugPrimitive();

		// �C��f�o�b�O�v���~�e�B�u�`��
		CanonManager::Instance().DrawDebugPrimitive();
		// �e�f�o�b�O�v���~�e�B�u�`��
		CanonBallManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{

	}

	// 2D�f�o�b�OGUI�`��
	{
		//�@�v���C���[�f�o�b�O�`��
		player->DrawDebugGUI();
		//�@�J�����R���g���[���[�f�o�b�O�`��
		cameraController->DrawDebugGUI();

		//�@ImGui�`��
		HoleManager::Instance().DrawDebugGUI();

		CanonManager::Instance().DrawDebugGUI();
	}
}

void SceneGame::SetMap(int Floor[20][20], int x, int z, int y)
{
	//���̔z�u
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

