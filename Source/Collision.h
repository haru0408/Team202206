#pragma once

#include <DirectXMath.h>

// ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
    // ‹…‚Æ‹…‚ÌŒğ·”»’è
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );
    // ‰~’Œ‚Æ‰~’Œ‚ÌŒğ·”»’è
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );
    // ‹…‚Æ‰~’Œ‚ÌŒğ·”»’è
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius, 
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    // ” ‚Æ” ‚Ì“–‚½‚è”»’è
    static bool IntersectBoxVsBox(
        const DirectX::XMFLOAT3& positionA,
        float widthA,
        float heightA,
        float lengthA,
        const DirectX::XMFLOAT3& positionB,
        float widthB,
        float heightB,
        float lengthB,
        DirectX::XMFLOAT3& outPositionA
    );
};