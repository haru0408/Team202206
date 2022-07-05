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
    // —Bˆê‚ÌƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
    static CanonManager& Instance()
    {
        static CanonManager instance;
        return instance;
    }

    // XVˆ—
    void Update(float elapsedTime);

    // •`‰æˆ—
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // –C‘ä“o˜^
    void Register(Canon* canon);

    // –C‘äíœ
    void Remove(Canon* canon);

    // –C‘ä‘Síœ
    void Clear();

    // ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
    void DrawDebugPrimitive();

    // –C‘ä”æ“¾
    int GetCanonCount() const { return static_cast<int>(canons.size()); }

    // –C‘äæ“¾
    Canon* GetCanon(int index) { return canons.at(index); }

private:
    std::vector<Canon*> canons;

    std::set<Canon*> removes;
};