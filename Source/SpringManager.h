#pragma once

#include <vector>
#include <set>

#include "Spring.h"

class SpringManager
{
private:
    SpringManager() {}
    ~SpringManager() { Clear(); }

public:
    // 唯一のインスタンス取得
    static SpringManager& Instance()
    {
        static SpringManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 砲台登録
    void Register(Spring* canon);

    // 砲台削除
    void Remove(Spring* canon);

    // 砲台全削除
    void Clear();

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 砲台数取得
    int GetSpringCount() const { return static_cast<int>(springs.size()); }

    // 砲台取得
    Spring* GetSpring(int index) { return springs.at(index); }

    // デバッグ用GUI描画
    void DrawDebugGUI();

private:
    std::vector<Spring*> springs;

    std::set<Spring*> removes;
};
