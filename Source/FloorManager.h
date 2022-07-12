#pragma once

#include <vector>
#include <set>
#include "Floor.h"

// エネミーマネージャー
class FloorManager
{
private:
    FloorManager() {}
    ~FloorManager() { Clear(); }

public:
    // 唯一のインスタンス取得
    static FloorManager& Instance()
    {
        static FloorManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // Floor登録
    void Register(Floor* floor);

    // Floor削除
    void Remove(Floor* floor);

    //Floor全削除
    void Clear();

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // Floor数取得
    int GetFloorCount() const { return static_cast<int>(floors.size()); }

    // Floor取得
    Floor* GetFloor(int index) { return floors.at(index); }

    int GetFloorNum() { return FloorNum; };
private:

    std::vector<Floor*> floors;
    int FloorNum;
    std::set<Floor*> removes;
};