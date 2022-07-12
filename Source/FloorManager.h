#pragma once

#include <vector>
#include <set>
#include "Floor.h"

// �G�l�~�[�}�l�[�W���[
class FloorManager
{
private:
    FloorManager() {}
    ~FloorManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static FloorManager& Instance()
    {
        static FloorManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // Floor�o�^
    void Register(Floor* floor);

    // Floor�폜
    void Remove(Floor* floor);

    //Floor�S�폜
    void Clear();

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // Floor���擾
    int GetFloorCount() const { return static_cast<int>(floors.size()); }

    // Floor�擾
    Floor* GetFloor(int index) { return floors.at(index); }

    int GetFloorNum() { return FloorNum; };
private:

    std::vector<Floor*> floors;
    int FloorNum;
    std::set<Floor*> removes;
};