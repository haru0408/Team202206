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

// ���Ɣ��̓����蔻��(�Ǘp)
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

    // A �� B�x�N�g�����v�Z
    DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, B);
    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    DirectX::XMFLOAT3 Normal;
    DirectX::XMStoreFloat3(&Normal, N);

    // �͈̓`�F�b�N
    if (fabsf(Normal.x) > fabsf(Normal.z))
    {
        // X����
        if (rightA < leftB) return false;
        if (rightB < leftA) return false;
        else XChack = true;
    }
    else if (fabsf(Normal.z) > fabsf(Normal.x))
    {
        // Z����
        if (frontB < backA) return false;
        if (frontA < backB) return false;
        else ZChack = true;
    }

    // B��A�������o��(XZ���ʂ̂�)
    // X����
    if (XChack)
    {
        if (Normal.x < 0) outPositionA.x = positionB.x - (LenA.x + LenB.x - Adjust);
        else if (Normal.x > 0) outPositionA.x = positionB.x + (LenA.x + LenB.x + Adjust);
        else                   outPositionA.x = positionA.x;

        outPositionA.y = positionA.y;
        outPositionA.z = positionA.z;
        return true;
    }
    // Z����
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

// ���Ɣ��̓����蔻��(�n�ʗp)
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

    // �͈̓`�F�b�N
    // X����
    if (rightA < leftB) return false;
    if (rightB < leftA) return false;
    // Y����
    if (topB < bottomA) return false;
    if (topA < bottomB) return false;
    // Z����
    if (frontB < backA) return false;
    if (frontA < backB) return false;

    // A��B�������o��(Y���ʂ̂�)
    // A��B�x�N�g�����v�Z
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