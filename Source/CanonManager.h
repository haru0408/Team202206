#pragma once

#include <vector>
#include <set>

#include "Canon.h"

class CanonManager
{
private:
    CanonManager() {}
    ~CanonManager() { Clear(); }

public:
    // �B��̃C���X�^���X�擾
    static CanonManager& Instance()
    {
        static CanonManager instance;
        return instance;
    }

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �C��o�^
    void Register(Canon* canon);

    // �C��폜
    void Remove(Canon* canon);

    // �C��S�폜
    void Clear();

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �C�䐔�擾
    int GetCanonCount() const { return static_cast<int>(canons.size()); }

    // �C��擾
    Canon* GetCanon(int index) { return canons.at(index); }

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

private:
    std::vector<Canon*> canons;

    std::set<Canon*> removes;
};