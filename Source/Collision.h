#pragma once

#include <DirectXMath.h>

// �R���W����
class Collision
{
public:
    // ���Ƌ��̌�������
    static bool IntersectSphereVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );
    // �~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );
    // ���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius, 
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    // ���Ɣ��̓����蔻��
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