#pragma once

#include "BaseScene.h"
#include <memory>

class SceneClear :public Scene
{
public:
    /* instance getter */
    static SceneClear& Instance() {
        static SceneClear instance;
        return instance;
    }

    //---------------------------//
private:
    SceneClear() {}
    ~SceneClear()override {}

    /* main method */
    void Initialize()override;
    void Finalize()override;
    void Update(float elapsedTime)override;
    void Render()override;

    /* get type */
    const SCENE_TYPE getType();

private:
    int state_ = 0;
    float timer_ = 0;
    std::unique_ptr<MenuBar> menu_ = nullptr;

    std::unique_ptr<AudioSource> BGM;

    const char* logoFile = ".\\Data\\Sprite\\LoadingIcon.png";
    const char* barFile = ".\\Data\\Sprite\\Title.png";
    const char* tezFile0 = "C:\\Users\\2210337\\Desktop\\iPhone‚æ‚è\\IMG_5828.JPG";
};
