#include "HoleManager.h"

// XV
void HoleManager::Update(float elapsedTime)
{
    for (Hole* hole : holes)
    {
        hole->Update(elapsedTime);
    }

    // jό
    for (Hole* hole : removes)
    {
        std::vector<Hole*>::iterator it = std::find(holes.begin(), holes.end(), hole);

        if (it != holes.end())
        {
            holes.erase(it);
        }

        delete hole;
    }
    // jόXgπNA
    removes.clear();
}

// o^
void HoleManager::Register(Hole* hole)
{
    holes.emplace_back(hole);
}

// ν
void HoleManager::Remove(Hole* hole)
{
    // jόXgΙΗΑ
    removes.insert(hole);
}

// Sν
void HoleManager::Clear()
{
    for (Hole* hole : holes)
    {
        delete hole;
    }
    holes.clear();
}

// fobOv~eBu`ζ
void HoleManager::DrawDebugPrimitive()
{
    for (Hole* hole : holes)
    {
        hole->DrawDebugPrimitive();
    }
}

// fobOpGUI`ζ
void HoleManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Hole", ImGuiTreeNodeFlags_DefaultOpen))
        {
            HoleManager& holeManager = Instance();
            int holeCount = holeManager.GetHoleCount();

            // PΜΜp[^
            for (int i = 0; i < holeCount; ++i)
            {
                Hole* hole = holeManager.GetHole(i);

                // int^πchar^ΙΟ·
                char holecount[5] = "0000";
                snprintf(holecount, 5, "%d", i);

                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode(holecount))
                {
                    // Κu
                    DirectX::XMFLOAT3 Position = hole->GetPosition();
                    ImGui::InputFloat3("Position", &Position.x);
                    hole->SetPosition(Position);

                    // Όa
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