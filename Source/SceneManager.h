#pragma once
/*
* Fuji:
*  class     Scene識別子
*  Manager   SceneManager
* -Attention    changeScene で DEBUG を使用できないようにしています。
*/

#include <cassert>

enum class SCENE_TYPE
{
    DEBUG
    , TITLE
    , GAME
    , CLEAR
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
    void Finalize();    //保険
    void Update(float elapsedTime);
    void Render();

    /* change scene */
    void changeScene(SCENE_TYPE next);

private:
    //SCENE_TYPE now_ = SCENE_TYPE::NONE;
    bool isChange_ = false; //遷移後の描画阻止用
    static Scene* scene;
};