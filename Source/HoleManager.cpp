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