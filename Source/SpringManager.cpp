#include "SpringManager.h"

// 更新処理
void SpringManager::Update(float elapsedTime)
{
    for (Spring* canon : springs)
    {
        canon->Update(elapsedTime);
    }

    // 破棄処理
    for (Spring* canon : removes)
    {
        std::vector<Spring*>::iterator it = std::find(springs.begin(), springs.end(), canon);

        if (it != springs.end())
        {
            springs.erase(it);
        }

        delete canon;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理
void SpringManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Spring* canon : springs)
    {
        canon->Render(dc, shader);
    }
}

// 砲台登録
void SpringManager::Register(Spring* canon)
{
    springs.emplace_back(canon);
}

// 砲台削除
void SpringManager::Remove(Spring* canon)
{
    // 破棄リストに追加
    removes.insert(canon);
}

// 砲台全削除
void SpringManager::Clear()
{
    for (Spring* canon : springs)
    {
        delete canon;
    }
    springs.clear();
}

// デバッグプリミティブ描画
void SpringManager::DrawDebugPrimitive()
{
    for (Spring* canon : springs)
    {
        canon->DrawDebugPrimitive();
    }
}
