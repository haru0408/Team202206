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

    // ���Ɣ��̓����蔻��(�Ǘp)
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

    // ���Ɣ��̓����蔻��(�n�ʗp)
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
    ); // ��true�Ȃ�velocity.y��0�ɂ��Ă�������
};