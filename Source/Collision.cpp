#include "Collision.h"

// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A->Bの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // 距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }

    // AがBを押し出す
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
    // Aの足元がBの頭より上なら当たっていない
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // Aの頭がBの足元より下なら当たっていない
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }
    // XZ平面での範囲チェック
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    float distXZ = vx * vx + vz * vz; //名前に反して二乗

    float range = radiusA + radiusB;
    if (distXZ > range * range)
    {
        return false;
    }

    // AがBを押し出す
    distXZ = sqrtf(distXZ); //名前に合わせるために平方根使用
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
    // Aの足元がBの頭より上なら外れ
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)
        return false;

    // Aの頭がBの足元より下なら外れ
    if (spherePosition.y - sphereRadius < cylinderPosition.y)
        return false;

    // XZ平面上の範囲チェック(円同士判定)
    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float distXZ = vx * vx + vz * vz; // 名前に反して二乗状態

    float range = sphereRadius + cylinderRadius;
    if (distXZ > range * range)
        return false;

    // AがBを押し出す(XZ平面のみ)
    distXZ = sqrtf(distXZ);  // 名前に合わせるために平方根仕様
    vx /= distXZ;
    vz /= distXZ;

    outCylinderPosition.x = spherePosition.x + (vx * range);
    outCylinderPosition.z = spherePosition.z + (vz * range);
    outCylinderPosition.y = spherePosition.y;

    return true;
}
