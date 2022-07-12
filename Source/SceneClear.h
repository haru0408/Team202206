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
    std::unique_ptr<MenuBar> menu_ = nullptr;
};
