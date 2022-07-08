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

// �f�o�b�O�pGUI�`��
void CanonManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Canon", ImGuiTreeNodeFlags_DefaultOpen))
        {
            CanonManager& canonManager = Instance();
            int canonCount = canonManager.GetCanonCount();

            // ���P�̂̃p�����[�^
            for (int i = 0; i < canonCount; ++i)
            {
                Canon* canon = canonManager.GetCanon(i);

                // int�^��char�^�ɕϊ�
                char canoncount[5] = "0000";
                snprintf(canoncount, 5, "%d", i);

                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode(canoncount))
                {
                    // �ʒu
                    DirectX::XMFLOAT3 Position = canon->GetPosition();
                    ImGui::InputFloat3("Position ", &Position.x);
                    canon->SetPosition(Position);

                    // ����
                    if (ImGui::Button("UP"))    canon->SetUpDirection();
                    ImGui::SameLine();
                    if (ImGui::Button("DOWN"))  canon->SetDownDirection();
                    ImGui::SameLine();
                    if (ImGui::Button("LEFT"))  canon->SetLeftDirection();
                    ImGui::SameLine();
                    if (ImGui::Button("RIGHT")) canon->SetRightDirection();
                    ImGui::SameLine();
                    ImGui::Text("  Direction");
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}