#include "FloorManager.h"
#include "Collision.h"

// �X�V����
void FloorManager::Update(float elapsedTime)
{
    for (Floor* floor : floors)
    {
        floor->Update(elapsedTime);
    }

    // �j������
    for (Floor* floor : removes)
    {
        std::vector<Floor*>::iterator it = std::find(floors.begin(), floors.end(), floor);

        if (it != floors.end())
        {
            floors.erase(it);
        }

        delete floor;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void FloorManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Floor* floor : floors)
    {
        floor->Render(dc, shader);
    }
}

//Floor�o�^
void FloorManager::Register(Floor* floor)
{
    FloorNum = floor->GetFloorNum();
    floors.emplace_back(floor);
}

// Floor�폜
void FloorManager::Remove(Floor* floor)
{
    // �j�����X�g�ɒǉ�
    removes.insert(floor);
}

//Floor�S�폜
void FloorManager::Clear()
{
    for (Floor* floor : floors)
    {
        delete floor;
    }
    floors.clear();
}

// �f�o�b�O�v���~�e�B�u�`��
void FloorManager::DrawDebugPrimitive()
{
    for (Floor* enemy : floors)
    {
        enemy->DrawDebugPrimitive();
    }
}