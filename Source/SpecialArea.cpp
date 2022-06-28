#include "SpecialArea.h"

#include "Collision.h"

//debug

inline void AreaWindow::DrawDebugPrimitive()
{
    const auto& rad = GetRadius();
    Graphics::Instance().GetDebugRenderer()->DrawBox(
        GetPosition()
        , rad.x
        , rad.y
        , rad.z
        , DirectX::XMFLOAT4(0.25f, 0.5f, 0.0f, 1.0f)
    );
}

//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//

/* main method */

inline void AreaManager::Update(float elapsedTime)
{
    /* window */
    {
        // update()
        for (auto area : windows_)
        {
            area->Update(elapsedTime);
        }

        // delete method
        for (auto it : removes_w_)
        {
            auto itr = std::find(windows_.begin(), windows_.end(), it);
            if (itr != windows_.end())
            {
                windows_.erase(itr);
            }
            delete it;
        }
        removes_w_.clear();
    }
}

inline void AreaManager::Render(ID3D11RenderTargetView* dc, Shader* shader)
{
    for (auto it : windows_)
    {
        it->Render(dc, shader);
    }
}

inline void AreaManager::Clear()
{
    for (auto it : windows_)
    {
        if (it != nullptr)
        {
            delete it;
        }
    }
    windows_.clear();
}

//

inline void AreaManager::CollisionAreaVsCharacter(Character* obj)
{
    /* window */
    {
        for (auto it : windows_)
        {
            auto& rad = it->GetRadius();
            if (Collision::IntersectBoxVsBox_Wall(
                it->GetPosition()
                , rad.x
                , rad.y
                , rad.z
                , obj->GetPosition()
                , obj->GetRadius()
                , obj->GetHeight()
                , obj->GetRadius()
                , DirectX::XMFLOAT3() //�Ȃ��ƃG���[�f���̂�
            ))
            {
                //auto v = obj->GetVelocity();
                //v.z -= it->GetPower();
                //obj->SetVelocity(v);
            }
        }
    }
}

/* register, delete */

inline void AreaManager::Register(AreaWindow* area)
{
    if (area != nullptr)
    {
        windows_.emplace_back(area);
    }
}

inline void AreaManager::Remove(AreaWindow* area)
{
    if (area != nullptr)
    {
        removes_w_.emplace_back(area);
    }
}

/* debug */

inline void AreaManager::DrawDebugPrimitive()
{
    for (auto it : windows_)
    {
        it->DrawDebugPrimitive();
    }
}

