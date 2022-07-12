#include "FloorManager.h"
#include "Collision.h"

// 更新処理
void FloorManager::Update(float elapsedTime)
{
    for (Floor* floor : floors)
    {
        floor->Update(elapsedTime);
    }

    // 破棄処理
    for (Floor* floor : removes)
    {
        std::vector<Floor*>::iterator it = std::find(floors.begin(), floors.end(), floor);

        if (it != floors.end())
        {
            floors.erase(it);
        }

        delete floor;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理
void FloorManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Floor* floor : floors)
    {
        floor->Render(dc, shader);
    }
}

//Floor登録
void FloorManager::Register(Floor* floor)
{
    FloorNum = floor->GetFloorNum();
    floors.emplace_back(floor);
}

// Floor削除
void FloorManager::Remove(Floor* floor)
{
    // 破棄リストに追加
    removes.insert(floor);
}

//Floor全削除
void FloorManager::Clear()
{
    for (Floor* floor : floors)
    {
        delete floor;
    }
    floors.clear();
}

// デバッグプリミティブ描画
void FloorManager::DrawDebugPrimitive()
{
    for (Floor* enemy : floors)
    {
        enemy->DrawDebugPrimitive();
    }
}