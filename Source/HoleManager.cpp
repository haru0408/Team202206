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

// �f�o�b�O�pGUI�`��
void HoleManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Hole", ImGuiTreeNodeFlags_DefaultOpen))
        {
            HoleManager& holeManager = Instance();
            int holeCount = holeManager.GetHoleCount();

            // ���P�̂̃p�����[�^
            for (int i = 0; i < holeCount; ++i)
            {
                Hole* hole = holeManager.GetHole(i);

                // int�^��char�^�ɕϊ�
                char holecount[5] = "0000";
                snprintf(holecount, 5, "%d", i);

                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode(holecount))
                {
                    // �ʒu
                    DirectX::XMFLOAT3 Position = hole->GetPosition();
                    ImGui::InputFloat3("Position", &Position.x);
                    hole->SetPosition(Position);

                    // ���a
                    float Radius = hole->GetRadius();
                    ImGui::InputFloat("Radius", &Radius);
                    hole->SetRadius(Radius);
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}