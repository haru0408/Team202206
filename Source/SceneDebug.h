#pragma once
/*
* Fuji:
*  class     SceneDebug
*  Manager   - Nothing -
* -Attention    - Nothing -
*/

#include "BaseScene.h"


class SceneDebug :public Scene
{
public:
    /* instance getter */
    static SceneDebug& Instance() {
        static SceneDebug instance;
        return instance;
    }

    //---------------------------//
private:
    // constructer etc.
    SceneDebug() {}
    ~SceneDebug()override {}

    /* main method */
    void Initialize()override {}
    void Finalize()override {}
    void Update(float elapsedTime)override {}
    void Render()override {}

    /* get type */
    const SCENE_TYPE getType();
};