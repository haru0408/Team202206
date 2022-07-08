#include "CanonManager.h"

// 更新処理
void CanonManager::Update(float elapsedTime)
{
    for (Canon* canon : canons)
    {
        canon->Update(elapsedTime);
    }

    // 破棄処理
    for (Canon* canon : removes)
    {
        std::vector<Canon*>::iterator it = std::find(canons.begin(), canons.end(), canon);

        if (it != canons.end())
        {
            canons.erase(it);
        }

        delete canon;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理
void CanonManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Canon* canon : canons)
    {
        canon->Render(dc, shader);
    }
}

// 砲台登録
void CanonManager::Register(Canon* canon)
{
    canons.emplace_back(canon);
}

// 砲台削除
void CanonManager::Remove(Canon* canon)
{
    // 破棄リストに追加
    removes.insert(canon);
}

// 砲台全削除
void CanonManager::Clear()
{
    for (Canon* canon : canons)
    {
        delete canon;
    }
    canons.clear();
}

// デバッグプリミティブ描画
void CanonManager::DrawDebugPrimitive()
{
    for (Canon* canon : canons)
    {
        canon->DrawDebugPrimitive();
    }
}

// デバッグ用GUI描画
void CanonManager::DrawDebugGUI()
{
    if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Canon", ImGuiTreeNodeFlags_DefaultOpen))
        {
            CanonManager& canonManager = Instance();
            int canonCount = canonManager.GetCanonCount();

            // 穴単体のパラメータ
            for (int i = 0; i < canonCount; ++i)
            {
                Canon* canon = canonManager.GetCanon(i);

                // int型をchar型に変換
                char canoncount[5] = "0000";
                snprintf(canoncount, 5, "%d", i);

                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode(canoncount))
                {
                    // 位置
                    DirectX::XMFLOAT3 Position = canon->GetPosition();
                    ImGui::InputFloat3("Position ", &Position.x);
                    canon->SetPosition(Position);

                    // 向き
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