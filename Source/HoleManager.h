#pragma once

#include <vector>
#include <set>

#include "Hole.h"

class HoleManager
{
private:
    HoleManager() {}
    ~HoleManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static HoleManager& Instance()
    {
        static HoleManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // ���o�^
    void Register(Hole* hole);

    // ���폜
    void Remove(Hole* hole);

    // ���S�폜
    void Clear();

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �����擾
    int GetHoleCount() const { return static_cast<int>(holes.size()); }

    // ���擾
    Hole* GetHole(int index) { return holes.at(index); }

private:
    std::vector<Hole*> holes;

    std::set<Hole*> removes;
};