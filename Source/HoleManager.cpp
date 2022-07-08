#include "HoleManager.h"

// 更新処理
void HoleManager::Update(float elapsedTime)
{
    for (Hole* hole : holes)
    {
        hole->Update(elapsedTime);
    }

    // 破棄処理
    for (Hole* hole : removes)
    {
        std::vector<Hole*>::iterator it = std::find(holes.begin(), holes.end(), hole);

        if (it != holes.end())
        {
            holes.erase(it);
        }

        delete hole;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 穴登録
void HoleManager::Register(Hole* hole)
{
    holes.emplace_back(hole);
}

// 穴削除
void HoleManager::Remove(Hole* hole)
{
    // 破棄リストに追加
    removes.insert(hole);
}

// 穴全削除
void HoleManager::Clear()
{
    for (Hole* hole : holes)
    {
        delete hole;
    }
    holes.clear();
}

// デバッグプリミティブ描画
void HoleManager::DrawDebugPrimitive()
{
    for (Hole* hole : holes)
    {
        hole->DrawDebugPrimitive();
    }
}

// デバッグ用GUI描画
void HoleManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Hole", ImGuiTreeNodeFlags_DefaultOpen))
        {
            HoleManager& holeManager = Instance();
            int holeCount = holeManager.GetHoleCount();

            // 穴単体のパラメータ
            for (int i = 0; i < holeCount; ++i)
            {
                Hole* hole = holeManager.GetHole(i);

                // int型をchar型に変換
                char holecount[5] = "0000";
                snprintf(holecount, 5, "%d", i);

                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode(holecount))
                {
                    // 位置
                    DirectX::XMFLOAT3 Position = hole->GetPosition();
                    ImGui::InputFloat3("Position", &Position.x);
                    hole->SetPosition(Position);

                    // 半径
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