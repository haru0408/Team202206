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