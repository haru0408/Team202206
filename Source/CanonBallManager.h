#pragma once

#include <vector>
#include <set>

#include "CanonBall.h"

class CanonBallManager
{
private:
    CanonBallManager() {}
    ~CanonBallManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static CanonBallManager& Instance()
    {
        static CanonBallManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // ���o�^
    void Register(CanonBall* canonBall);

    // ���폜
    void Remove(CanonBall* canonBall);

    // ���S�폜
    void Clear();

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �����擾
    int GetCanonBallCount() const { return static_cast<int>(canonBalls.size()); }

    // ���擾
    CanonBall* GetCanonBall(int index) { return canonBalls.at(index); }

private:
    std::vector<CanonBall*> canonBalls;

    std::set<CanonBall*> removes;
};