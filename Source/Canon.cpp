#include "Canon.h"
#include "CanonManager.h"
#include "CanonBallManager.h"

Canon::Canon()
{
    model = new Model("Data/Model/Canon/Canon.mdl");

    // ���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;
}

Canon::~Canon()
{
    delete model;
}

// �X�V����
void Canon::Update(float elapsedTime)
{
    CanonBallManager& canonBallManager = CanonBallManager::Instance();

    // �e����
    if (LanchTimer < 0)
    {
        CanonBall* canonBall = new CanonBall();
        canonBall->SetPosition(DirectX::XMFLOAT3(position.x, position.y + (length.y * 0.35f), position.z));
        canonBallManager.Register(canonBall);

        LanchTimer = 60;
    }
    --LanchTimer;

    // �e�̑��x��ݒ肷��
    const float BallSpeed = 0.2f;
    int canonBallCount    = canonBallManager.GetCanonBallCount();
    for (int i = 0; i < canonBallCount; ++i)
    {
        CanonBall* canonBall = canonBallManager.GetCanonBall(i);

        if (canonBall->GetVelocity().x == 0.0f && canonBall->GetVelocity().z == 0.0f)
        {
            switch (direction)
            {
            case 1: // �����
                canonBall->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, BallSpeed));

                AdjustPosition = { 0.0f, 1.0f, -0.5f };
                DirectionAngle = DirectX::XMConvertToRadians(0.0f);

                break;

            case 2: // ������
                canonBall->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, -BallSpeed));

                AdjustPosition = { 0.0f, 1.0f, 0.5f };
                DirectionAngle = DirectX::XMConvertToRadians(180.0f);

                break;

            case 3: // ������
                canonBall->SetVelocity(DirectX::XMFLOAT3(-BallSpeed, 0.0f, 0.0f));

                AdjustPosition = { 0.5f, 1.0f, 0.0f };
                DirectionAngle = DirectX::XMConvertToRadians(-90.0f);

                break;

            case 4: // �E����
                canonBall->SetVelocity(DirectX::XMFLOAT3(BallSpeed, 0.0f, 0.0f));

                AdjustPosition = { -0.5f, 1.0f, 0.0f };
                DirectionAngle = DirectX::XMConvertToRadians(90.0f);

                break;
            }
        }
    }

    // �s��X�V����
    UpdateTransform(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
                    DirectX::XMFLOAT3(0.0f, DirectionAngle, 0.0f), 
                    AdjustPosition);

    // ���f���s����X�V
    model->UpdateTransform(transform);
}

// �s��X�V����
void Canon::UpdateTransform(const DirectX::XMFLOAT3& AdjustScale,
                            const DirectX::XMFLOAT3& AdjustAngle,
                            const DirectX::XMFLOAT3& AdjustPosition)
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x + AdjustScale.x,
                                                   scale.y + AdjustScale.y,
                                                   scale.z + AdjustScale.z);

    // ��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x + AdjustAngle.x,
                                                                angle.y + AdjustAngle.y,
                                                                angle.z + AdjustAngle.z);
                                                                
    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + AdjustPosition.x,
                                                       position.y + AdjustPosition.y,
                                                       position.z + AdjustPosition.z);

    // �R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

// �`�揈��
void Canon::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

// �f�o�b�O�v���~�e�B�u�`��
void Canon::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawBox(position, length.x, length.y, length.z, DirectX::XMFLOAT4(1, 0, 1, 1));
}

// �j��
void Canon::Destroy()
{
    CanonManager::Instance().Remove(this);
}