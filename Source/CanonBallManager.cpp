#include "CanonBallManager.h"

// �X�V����
void CanonBallManager::Update(float elapsedTime)
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->Update(elapsedTime);
    }

    // �j������
    for (CanonBall* canonBall : removes)
    {
        std::vector<CanonBall*>::iterator it = std::find(canonBalls.begin(), canonBalls.end(), canonBall);

        if (it != canonBalls.end())
        {
            canonBalls.erase(it);
        }

        delete canonBall;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void CanonBallManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->Render(dc, shader);
    }
}

// �C��o�^
void CanonBallManager::Register(CanonBall* canonBall)
{
    canonBalls.emplace_back(canonBall);
}

// �C��폜
void CanonBallManager::Remove(CanonBall* canonBall)
{
    // �j�����X�g�ɒǉ�
    removes.insert(canonBall);
}

// �C��S�폜
void CanonBallManager::Clear()
{
    for (CanonBall* canonBall : canonBalls)
    {
        delete canonBall;
    }
    canonBalls.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void CanonBallManager::DrawDebugPrimitive()
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->DrawDebugPrimitive();
    }
}