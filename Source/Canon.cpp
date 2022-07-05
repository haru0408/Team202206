#include "Canon.h"
#include "CanonManager.h"
#include "CanonBallManager.h"

// �X�V����
void Canon::Update(float elapsedTime)
{
    CanonBallManager& canonBallManager = CanonBallManager::Instance();

    // �e����
    if (LanchTimer < 0)
    {
        CanonBall* canonBall = new CanonBall();
        canonBall->SetPosition(DirectX::XMFLOAT3(position.x, position.y + (length.y * 0.5f), position.z));
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

                break;

            case 2: // ������
                canonBall->SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, -BallSpeed));

                break;

            case 3: // ������
                canonBall->SetVelocity(DirectX::XMFLOAT3(-BallSpeed, 0.0f, 0.0f));

                break;

            case 4: // �E����
                canonBall->SetVelocity(DirectX::XMFLOAT3(BallSpeed, 0.0f, 0.0f));

                break;
            }
        }
    }
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