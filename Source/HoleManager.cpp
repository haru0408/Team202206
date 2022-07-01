#include "HoleManager.h"

// �X�V����
void HoleManager::Update(float elapsedTime)
{
    for (Hole* hole : holes)
    {
        hole->Update(elapsedTime);
    }

    // �j������
    for (Hole* hole : removes)
    {
        std::vector<Hole*>::iterator it = std::find(holes.begin(), holes.end(), hole);

        if (it != holes.end())
        {
            holes.erase(it);
        }

        delete hole;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// ���o�^
void HoleManager::Register(Hole* hole)
{
    holes.emplace_back(hole);
}

// ���폜
void HoleManager::Remove(Hole* hole)
{
    // �j�����X�g�ɒǉ�
    removes.insert(hole);
}

// ���S�폜
void HoleManager::Clear()
{
    for (Hole* hole : holes)
    {
        delete hole;
    }
    holes.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void HoleManager::DrawDebugPrimitive()
{
    for (Hole* hole : holes)
    {
        hole->DrawDebugPrimitive();
    }
}