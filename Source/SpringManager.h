#pragma once

#include <vector>
#include <set>

#include "Spring.h"

class SpringManager
{
private:
    SpringManager() {}
    ~SpringManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static SpringManager& Instance()
    {
        static SpringManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �C��o�^
    void Register(Spring* canon);

    // �C��폜
    void Remove(Spring* canon);

    // �C��S�폜
    void Clear();

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �C�䐔�擾
    int GetSpringCount() const { return static_cast<int>(springs.size()); }

    // �C��擾
    Spring* GetSpring(int index) { return springs.at(index); }

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

private:
    std::vector<Spring*> springs;

    std::set<Spring*> removes;
};
