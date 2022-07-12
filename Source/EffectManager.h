#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

class EffectManager
{
private:
    EffectManager() {}
    ~EffectManager() {}

public:
    // �B��̃C���X�^���X�擾
    static EffectManager& Instance()
    {
        static EffectManager instance;
        return instance;
    }

    // ������
    void Initialize();

    // �I����
    void Finelize();

    // �X�V����
    void Update(float elapsedTime);

    // �`�揈��
    void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // Effekseer�}�l�[�W���[�̎擾
    Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }

private:
    Effekseer::ManagerRef effekseerManager;
    EffekseerRenderer::RendererRef effekseerRenderer;
};