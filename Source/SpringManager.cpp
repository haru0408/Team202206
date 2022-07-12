#include "SpringManager.h"

// �X�V����
void SpringManager::Update(float elapsedTime)
{
    for (Spring* canon : springs)
    {
        canon->Update(elapsedTime);
    }

    // �j������
    for (Spring* canon : removes)
    {
        std::vector<Spring*>::iterator it = std::find(springs.begin(), springs.end(), canon);

        if (it != springs.end())
        {
            springs.erase(it);
        }

        delete canon;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void SpringManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Spring* canon : springs)
    {
        canon->Render(dc, shader);
    }
}

// �C��o�^
void SpringManager::Register(Spring* canon)
{
    springs.emplace_back(canon);
}

// �C��폜
void SpringManager::Remove(Spring* canon)
{
    // �j�����X�g�ɒǉ�
    removes.insert(canon);
}

// �C��S�폜
void SpringManager::Clear()
{
    for (Spring* canon : springs)
    {
        delete canon;
    }
    springs.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void SpringManager::DrawDebugPrimitive()
{
    for (Spring* canon : springs)
    {
        canon->DrawDebugPrimitive();
    }
}
