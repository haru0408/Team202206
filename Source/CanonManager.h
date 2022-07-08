#pragma once

#include <vector>
#include <set>

#include "Canon.h"

class CanonManager
{
private:
    CanonManager() {}
    ~CanonManager() { Clear(); }

public:
    // 唯一のインスタンス取得
    static CanonManager& Instance()
    {
        static CanonManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 砲台登録
    void Register(Canon* canon);

    // 砲台削除
    void Remove(Canon* canon);

    // 砲台全削除
    void Clear();

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 砲台数取得
    int GetCanonCount() const { return static_cast<int>(canons.size()); }

    // 砲台取得
    Canon* GetCanon(int index) { return canons.at(index); }

    // デバッグ用GUI描画
    void DrawDebugGUI();

private:
    std::vector<Canon*> canons;

    std::set<Canon*> removes;
};