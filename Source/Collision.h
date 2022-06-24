#pragma once

#include <DirectXMath.h>

// コリジョン
class Collision
{
public:
    // 球と球の交差判定
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );
    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );
    // 球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius, 
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    // 箱と箱の当たり判定(壁用)
    static bool IntersectBoxVsBox_Wall(
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

    // 箱と箱の当たり判定(地面用)
    static bool IntersectBoxVsBox_Ground(
        const DirectX::XMFLOAT3& positionA,
        float widthA,
        float heightA,
        float lengthA,
        const DirectX::XMFLOAT3& positionB,
        float widthB,
        float heightB,
        float lengthB,
        DirectX::XMFLOAT3& outPositionA
    ); // ※trueならvelocity.yを0にしてください
};