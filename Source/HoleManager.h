#pragma once

#include <vector>
#include <set>

#include "Hole.h"

class HoleManager
{
private:
    HoleManager() {}
    ~HoleManager() { Clear(); }

public:
    // 唯一のインスタンス取得
    static HoleManager& Instance()
    {
        static HoleManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 穴登録
    void Register(Hole* hole);

    // 穴削除
    void Remove(Hole* hole);

    // 穴全削除
    void Clear();

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 穴数取得
    int GetHoleCount() const { return static_cast<int>(holes.size()); }

    // 穴取得
    Hole* GetHole(int index) { return holes.at(index); }

private:
    std::vector<Hole*> holes;

    std::set<Hole*> removes;
};