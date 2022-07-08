#pragma once

#include <vector>
#include <set>

#include "CanonBall.h"

class CanonBallManager
{
private:
    CanonBallManager() {}
    ~CanonBallManager() { Clear(); }

public:
    // 唯一のインスタンス取得
    static CanonBallManager& Instance()
    {
        static CanonBallManager instance;
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 穴登録
    void Register(CanonBall* canonBall);

    // 穴削除
    void Remove(CanonBall* canonBall);

    // 穴全削除
    void Clear();

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 穴数取得
    int GetCanonBallCount() const { return static_cast<int>(canonBalls.size()); }

    // 穴取得
    CanonBall* GetCanonBall(int index) { return canonBalls.at(index); }

private:
    std::vector<CanonBall*> canonBalls;

    std::set<CanonBall*> removes;
};