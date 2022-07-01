#pragma once
/*
* Fuji:
*  class     SceneÇÃÉxÅ[ÉX
*  Manager   - Nothing -
* -Attention    - Nothing -
*/


enum class SCENE_TYPE;
class Scene
{
protected:
    // constructer etc.
    Scene() {}
    virtual ~Scene() {}
public:
    /* main method */
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;

    /* get type */
    virtual const SCENE_TYPE getType() = 0;
};

