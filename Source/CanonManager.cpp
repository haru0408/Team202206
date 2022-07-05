#include "CanonManager.h"

// �X�V����
void CanonManager::Update(float elapsedTime)
{
    for (Canon* canon : canons)
    {
        canon->Update(elapsedTime);
    }

    // �j������
    for (Canon* canon : removes)
    {
        std::vector<Canon*>::iterator it = std::find(canons.begin(), canons.end(), canon);

        if (it != canons.end())
        {
            canons.erase(it);
        }

        delete canon;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void CanonManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Canon* canon : canons)
    {
        canon->Render(dc, shader);
    }
}

// �C��o�^
void CanonManager::Register(Canon* canon)
{
    canons.emplace_back(canon);
}

// �C��폜
void CanonManager::Remove(Canon* canon)
{
    // �j�����X�g�ɒǉ�
    removes.insert(canon);
}

// �C��S�폜
void CanonManager::Clear()
{
    for (Canon* canon : canons)
    {
        delete canon;
    }
    canons.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void CanonManager::DrawDebugPrimitive()
{
    for (Canon* canon : canons)
    {
        canon->DrawDebugPrimitive();
    }
}