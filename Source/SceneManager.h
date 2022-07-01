#pragma once
/*
* Fuji:
*  class     Scene���ʎq
*  Manager   SceneManager
* -Attention    changeScene �� DEBUG ���g�p�ł��Ȃ��悤�ɂ��Ă��܂��B
*/

#include <cassert>

enum class SCENE_TYPE
{
    DEBUG
    , TITLE
    , GAME
};

class Scene;
class SceneManager
{
private:
    // constructer etc.
    SceneManager() {}
    ~SceneManager() {}
public:
    /* instance getter */
    static SceneManager& Instance() {
        static SceneManager instance;
        return instance;
    }

    /* main method */
    void Finalize();    //�ی�
    void Update(float elapsedTime);
    void Render();

    /* change scene */
    void changeScene(SCENE_TYPE next);

private:
    //SCENE_TYPE now_ = SCENE_TYPE::NONE;
    bool isChange_ = false; //�J�ڌ�̕`��j�~�p
    static Scene* scene;
};