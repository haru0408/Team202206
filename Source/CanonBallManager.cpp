#include "CanonBallManager.h"

// 更新処理
void CanonBallManager::Update(float elapsedTime)
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->Update(elapsedTime);
    }

    // 破棄処理
    for (CanonBall* canonBall : removes)
    {
        std::vector<CanonBall*>::iterator it = std::find(canonBalls.begin(), canonBalls.end(), canonBall);

        if (it != canonBalls.end())
        {
            canonBalls.erase(it);
        }

        delete canonBall;
    }
    // 破棄リストをクリア
    removes.clear();
}

// 描画処理
void CanonBallManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->Render(dc, shader);
    }
}

// 砲台登録
void CanonBallManager::Register(CanonBall* canonBall)
{
    canonBalls.emplace_back(canonBall);
}

// 砲台削除
void CanonBallManager::Remove(CanonBall* canonBall)
{
    // 破棄リストに追加
    removes.insert(canonBall);
}

// 砲台全削除
void CanonBallManager::Clear()
{
    for (CanonBall* canonBall : canonBalls)
    {
        delete canonBall;
    }
    canonBalls.clear();
}

// デバッグプリミティブ描画
void CanonBallManager::DrawDebugPrimitive()
{
    for (CanonBall* canonBall : canonBalls)
    {
        canonBall->DrawDebugPrimitive();
    }
}