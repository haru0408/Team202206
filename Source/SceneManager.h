#pragma once
/*
* Fuji:
*  class     Scene¯•Êq
*  Manager   SceneManager
* -Attention    changeScene ‚Å DEBUG ‚ğg—p‚Å‚«‚È‚¢‚æ‚¤‚É‚µ‚Ä‚¢‚Ü‚·B
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
    void Finalize();    //•ÛŒ¯
    void Update(float elapsedTime);
    void Render();

    /* change scene */
    void changeScene(SCENE_TYPE next);

private:
    //SCENE_TYPE now_ = SCENE_TYPE::NONE;
    bool isChange_ = false; //‘JˆÚŒã‚Ì•`‰æ‘j~—p
    static Scene* scene;
};