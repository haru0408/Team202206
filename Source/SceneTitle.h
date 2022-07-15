#pragma once
/*
* Fuji:
*  class     SceneTitle, 2D‰æ‘œ
*  Manager   - Nothing -
* -Attention    - Nothing -
*/


#include "BaseScene.h"

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
//#include "Stage.h"
//#include "Player.h"
//#include "CameraController.h"



class SceneTitle:public Scene
{
public:
    /* instance getter */
    static SceneTitle& Instance() {
        static SceneTitle instance;
        return instance;
    }

    //---------------------------//
private:
    // constructer etc.
    SceneTitle() {}
    ~SceneTitle()override {}

    /* main method */
    void Initialize()override;
    void Finalize()override;
    void Update(float elapsedTime)override;
    void Render()override;

    /* get type */
    const SCENE_TYPE getType();

private:
    std::unique_ptr<MenuBar> menu_ = nullptr;

    std::unique_ptr<AudioSource> BGM;
};

