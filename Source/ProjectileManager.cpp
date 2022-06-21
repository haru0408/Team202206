#include "ProjectileManager.h"

// �R���X�g���N�^
ProjectileManager::ProjectileManager()
{

}

// �f�X�g���N�^
ProjectileManager::~ProjectileManager()
{
    Clear();
}

// �X�V����
void ProjectileManager::Update(float elapsedTime)
{
    // �X�V����
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    // �j������
    // ��projectiles�͈̔�for������erase()����ƕs�����������̂�
    // �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
    for (Projectile* projectile : removes)
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�ō폜���Ȃ���΂Ȃ�Ȃ�
        std::vector<Projectile*>::iterator it = std::find(  projectiles.begin(), 
                                                            projectiles.end(), 
                                                            projectile);

        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        // �e�ۂ̔j������
        delete projectile;
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �`�揈��
void ProjectileManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(context, shader);
    }
}

// �f�o�b�O�v���~�e�B�u�`��
void ProjectileManager::DrawDebugPrimitive()
{
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugPrimitive();
    }
}

// �e�ۓo�^
void ProjectileManager::Register(Projectile* projectile)
{
    if (projectile != nullptr)
    {
        projectiles.emplace_back(projectile);
    }
}

// �e�ۑS�폜
void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)
    {
        if (projectile != nullptr)
        {
            delete projectile; //projectiles���w���f�[�^������
        }
    }

    projectiles.clear(); //projectiles���̂��̂�����
}

void ProjectileManager::Remove(Projectile* projectile)
{
    removes.insert(projectile);
}