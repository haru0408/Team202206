#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A->B�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    // A��B�������o��
    Vec = DirectX::XMVector3Normalize(Vec);
    Vec = DirectX::XMVectorScale(Vec, range);
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);
    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true;
}

bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }
    // XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    float distXZ = vx * vx + vz * vz; //���O�ɔ����ē��

    float range = radiusA + radiusB;
    if (distXZ > range * range)
    {
        return false;
    }

    // A��B�������o��
    distXZ = sqrtf(distXZ); //���O�ɍ��킹�邽�߂ɕ������g�p
    vx /= distXZ;
    vz /= distXZ;

    outPositionB.x = positionA.x + (vx * range);
    outPositionB.z = positionA.z + (vz * range);
    outPositionB.y = positionB.y;

    return true;
}  

bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition)
{
    // A�̑�����B�̓�����Ȃ�O��
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
        return false;

    // A�̓���B�̑�����艺�Ȃ�O��
    if (spherePosition.y - sphereRadius < cylinderPosition.y)
        return false;

    // XZ���ʏ�͈̔̓`�F�b�N(�~���m����)
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float distXZ = vx * vx + vz * vz; // ���O�ɔ����ē����

    float range = sphereRadius + cylinderRadius;
    if (distXZ > range * range)
        return false;

    // A��B�������o��(XZ���ʂ̂�)
    distXZ = sqrtf(distXZ);  // ���O�ɍ��킹�邽�߂ɕ������d�l
    vx /= distXZ;
    vz /= distXZ;

    outCylinderPosition.x = spherePosition.x + (vx * range);
    outCylinderPosition.z = spherePosition.z + (vz * range);
    outCylinderPosition.y = spherePosition.y;

    return true;
}
