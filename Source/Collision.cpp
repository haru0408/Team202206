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

// 箱と箱の当たり判定(壁用)
bool Collision::IntersectBoxVsBox_Wall(
    const DirectX::XMFLOAT3& positionA,
    float widthA,
    float heightA,
    float lengthA,
    const DirectX::XMFLOAT3& positionB,
    float widthB,
    float heightB,
    float lengthB,
    DirectX::XMFLOAT3& outPositionA)
{
    const float Adjust = 0.001f;

    DirectX::XMFLOAT3 LenA = { widthA / 2, heightA / 2, lengthA / 2 };
    DirectX::XMFLOAT3 LenB = { widthB / 2, heightB / 2, lengthB / 2 };

    float rightA = positionA.x + LenA.x;
    float leftA = positionA.x - LenA.x;
    float topA = positionA.y + heightA;
    float bottomA = positionA.y;
    float frontA = positionA.z + LenA.z;
    float backA = positionA.z - LenA.z;

    float rightB = positionB.x + LenB.x;
    float leftB = positionB.x - LenB.x;
    float topB = positionB.y + heightB;
    float bottomB = positionB.y;
    float frontB = positionB.z + LenB.z;
    float backB = positionB.z - LenB.z;

    bool XChack = false;
    bool ZChack = false;

    // A → Bベクトルを計算
    DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, B);
    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    DirectX::XMFLOAT3 Normal;
    DirectX::XMStoreFloat3(&Normal, N);

    // 範囲チェック
    if (fabsf(Normal.x) > fabsf(Normal.z))
    {
        // X平面
        if (rightA < leftB) return false;
        if (rightB < leftA) return false;
        else XChack = true;
    }
    else if (fabsf(Normal.z) > fabsf(Normal.x))
    {
        // Z平面
        if (frontB < backA) return false;
        if (frontA < backB) return false;
        else ZChack = true;
    }

    // BがAを押し出す(XZ平面のみ)
    // X平面
    if (XChack)
    {
        if (Normal.x < 0) outPositionA.x = positionB.x - (LenA.x + LenB.x - Adjust);
        else if (Normal.x > 0) outPositionA.x = positionB.x + (LenA.x + LenB.x + Adjust);
        else                   outPositionA.x = positionA.x;

        outPositionA.y = positionA.y;
        outPositionA.z = positionA.z;
        return true;
    }
    // Z平面
    if (ZChack)
    {
        if (Normal.z > 0) outPositionA.z = positionB.z + (LenA.z + LenB.z + Adjust);
        else if (Normal.z < 0) outPositionA.z = positionB.z - (LenA.z + LenB.z - Adjust);
        else                   outPositionA.z = positionA.z;

        outPositionA.x = positionA.x;
        outPositionA.y = positionA.y;
        return true;
    }

    return true;
}

// 箱と箱の当たり判定(地面用)
bool Collision::IntersectBoxVsBox_Ground(
    const DirectX::XMFLOAT3& positionA,
    float widthA,
    float heightA,
    float lengthA,
    const DirectX::XMFLOAT3& positionB,
    float widthB,
    float heightB,
    float lengthB,
    DirectX::XMFLOAT3& outPositionA
)
{
    const float Adjust = 0.001f;

    DirectX::XMFLOAT3 LenA = { widthA / 2, heightA / 2, lengthA / 2 };
    DirectX::XMFLOAT3 LenB = { widthB / 2, heightB / 2, lengthB / 2 };

    float rightA = positionA.x + LenA.x;
    float leftA = positionA.x - LenA.x;
    float topA = positionA.y + heightA;
    float bottomA = positionA.y;
    float frontA = positionA.z + LenA.z;
    float backA = positionA.z - LenA.z;

    float rightB = positionB.x + LenB.x;
    float leftB = positionB.x - LenB.x;
    float topB = positionB.y + heightB;
    float bottomB = positionB.y;
    float frontB = positionB.z + LenB.z;
    float backB = positionB.z - LenB.z;

    outPositionA.y = positionA.y;
    outPositionA.x = positionA.x;
    outPositionA.z = positionA.z;

    // 範囲チェック
    // X平面
    if (rightA < leftB) return false;
    if (rightB < leftA) return false;
    // Y平面
    if (topB < bottomA) return false;
    if (topA < bottomB) return false;
    // Z平面
    if (frontB < backA) return false;
    if (frontA < backB) return false;

    // AがBを押し出す(Y平面のみ)
    // A→Bベクトルを計算
    DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, B);
    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    DirectX::XMFLOAT3 Normal;
    DirectX::XMStoreFloat3(&Normal, N);

    if (Normal.y > 0.0f) outPositionA.y = positionB.y + (heightB + Adjust);
    else if (Normal.y < 0.0f) outPositionA.y = positionB.y - (heightB - Adjust);
    else outPositionA.y = positionA.y;

    return true;
}