#pragma once
/*
* Fuji:
*  class     Scene�̃x�[�X
*  Manager   - Nothing -
* -Attention    - Nothing -
*/

#include <functional>
#include <DirectXMath.h>
#include <vector>

#include "Audio/Audio.h"

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

//------------------------//

#include "Graphics/Sprite.h"
#include <memory>
enum class SPRITE_PIVOT {
    LT,CT,RT
    ,LM,CM,RM
    ,LB,CB,RB
};
class Sprite_batch :public Sprite
{
private:
    const char* filename_ = nullptr;
    std::vector<Vertex> vertices;
    size_t max_vertexCnt = 0;
public:
    Sprite_batch(const char* filename, size_t max_sprites = 1);
    //Sprite_batch(const char* filename) :Sprite(filename) {}
    void begin(ID3D11DeviceContext* immediate_context);
    void end(ID3D11DeviceContext* immediate_context);

    void Render(ID3D11DeviceContext* dc,
        float dx, float dy,
        float dw, float dh,
        float sx, float sy,
        float sw, float sh,
        float angle,
        float r, float g, float b, float a);
    const char* GetFilename()const { return filename_; }
};
class Texture
{
public:
    // constructer etc.
    Texture(
        const char* filename
        , size_t maxCnt
        , const DirectX::XMFLOAT2& pos = {}
        , SPRITE_PIVOT pivot = SPRITE_PIVOT::LT
        , const DirectX::XMFLOAT2& scale = { 1,1 }
        , int zCode = 0
        , const DirectX::XMFLOAT2& texPos = {}
        , const DirectX::XMFLOAT2& texSize = {}
        , float angle = 0.0f
        , const DirectX::XMFLOAT4& color = { 1,1,1,1 }
    );
    Texture(Texture&) = delete;

    /* main method */
    /// <param name="p">��_</param>
    void Render(const DirectX::XMFLOAT2& p);

    //getter
    const DirectX::XMFLOAT2& GetPosition()const { return position_; }
    const DirectX::XMFLOAT2& GetScale()const { return scale_; }
    const DirectX::XMFLOAT2& GetTexPos()const { return texPos_; }
    const DirectX::XMFLOAT2& GetTexSize()const { return texSize_; }
    float GetAngle()const { return angle_; }
    const DirectX::XMFLOAT4& GetColor()const { return color_; }
    int GetZ()const { return zCode_; }
    Sprite_batch* GetFile()const { return file_.get(); }

    //setter
    void SetScale(const DirectX::XMFLOAT2& s) { scale_ = s; }
    void SetColor(const DirectX::XMFLOAT4& c) { color_ = c; }
    void SetAngle(float a) { angle_ = normalizeAngle(a); }
    void AddAngle(float a) { normalizeAngle(angle_ += a); }

private:
    std::shared_ptr<Sprite_batch> file_;
    DirectX::XMFLOAT2 position_{};
    SPRITE_PIVOT pivot_{};
    DirectX::XMFLOAT2 scale_{};
    DirectX::XMFLOAT2 texPos_{};
    DirectX::XMFLOAT2 texSize_{};
    float angle_{};
    DirectX::XMFLOAT4 color_{};
    int zCode_ = 0;
private:
    inline float normalizeAngle(float& angle)
    {
        if (angle < -DirectX::XM_PI) angle += DirectX::XM_2PI;
        if (DirectX::XM_PI < angle) angle -= DirectX::XM_2PI;
        return angle;
    }
};
class TextInput
{
private:
    struct TimerSets
    {
        TimerSets(
            float* timer
            , DirectX::XMFLOAT2 pos = { 0,0 }
            , DirectX::XMFLOAT2 scale = { 1,1 }
            , int decimalP = 2
        )
        {
            addTime_ = timer, position_ = pos, scale_ = scale, decimalP_ = decimalP;
        }
        float* addTime_ = nullptr;
        float timer_ = 0.0f;
        DirectX::XMFLOAT2 position_{};
        DirectX::XMFLOAT2 scale_{};
        int decimalP_ = 2;
    };
    std::vector<std::shared_ptr<Texture>> text_{};
    std::vector<TimerSets> timers_{};
    std::vector<std::shared_ptr<Texture>> updates_{};
    struct FontData
    {
        const char* filename;
        DirectX::XMFLOAT2 fontSize;
    };
    FontData numFont = {
        "C:\\Users\\2210337\\Desktop\\�`�[������\\(2�NFirst)\\00���y2�E3�N���^���{�Q�[����܁w���G�x_6���}�X�^�^2022�N�x�O���^�Q�[������z_��o�ɂ���\\2038_GR2SB������_GR2GA�����S�� _GR2SB�n�ӗm_GR2SA��������\\1_�Q�[��\\Data\\Fonts\\font1.png"
        , { 32.0f,32.0f }
    };
public:
    TextInput() {}
    ~TextInput() {}

    //template<typename T>
    //void textout(T* text
    //    , DirectX::XMFLOAT2 pos = {}
    //    , DirectX::XMFLOAT2 scale = { 1,1 }
    //    , SPRITE_PIVOT pivot = SPRITE_PIVOT::LT
    //);
    void numRender(int num, DirectX::XMFLOAT2 scale = { 1,1 }, DirectX::XMFLOAT2 pos = {});
    /// <summary>
    /// 
    /// </summary>
    /// <param name="decimalP">�b�̏����_�ȉ��`�搔</param>
    void timeRender(float* time
        , DirectX::XMFLOAT2 pos = {}
        , DirectX::XMFLOAT2 scale = { 1,1 }
        , int decimalP = 2
    );
    void timeRender(bool update, float time
        , DirectX::XMFLOAT2 pos = {}
        , DirectX::XMFLOAT2 scale = { 1,1 }
        , int decimalP = 2
    );
    void timeRender(bool update, DirectX::XMFLOAT3 time
        , DirectX::XMFLOAT2 pos = {}
        , DirectX::XMFLOAT2 scale = { 1,1 }
        , int decimalP = 2
    );
    void timeRender(bool update,int hour, int minutes, float seconds
        , DirectX::XMFLOAT2 pos = {}
        , DirectX::XMFLOAT2 scale = { 1,1 }
        , int decimalP = 2
    );
    void Render();
}; static TextInput TEXTOUT;


class MenuBar
{
public:
    //constructer etc.
    /// <summary>
    /// 
    /// </summary>
    /// <param name="size">��`����p</param>
    /// <param name="vertical">��`���񎞏㉺�Ƀo�[�ړ��ł��邩�ۂ�</param>
    /// <param name="horizontal">��`���񎞍��E���E���ړ��ł��邩�ۂ�</param>
    MenuBar(const DirectX::XMFLOAT2& size, bool loop = true, bool horizontal = true, bool vertical = true)
        :loop_(loop), size_(size), moveVertical_(vertical), moveHorizontal_(horizontal)
    {}
    ~MenuBar() {
        if (logo_) delete logo_;
        if (BG_) delete BG_;
        for (auto it : subBar_)
        {
            if (it != nullptr)
                delete it;
        }
        subBar_.clear();
    }

    /* main method */
    void Update();
    void Render();

    //getter
    const std::vector<Texture*>* GetTexList() const { return &textures_; }
    const std::vector<Texture*>* GetBarList() const { return &subBar_; }

    //setter
    void SetLogo(Texture* s) { logo_ = s; }
    void SetBG(Texture* s) { BG_ = s; }
    void SetPosition(const DirectX::XMFLOAT2& p) { position_ = p; }
    //
    void AddSubBer(Texture* s, std::function<void()> func = []() {}) { subBar_.emplace_back(s); func_.emplace_back(func); }
    void AddTexture(Texture* s) { textures_.emplace_back(s); }
private:
    DirectX::XMFLOAT2 position_ = {};

    Texture* logo_ = nullptr;
    Texture* BG_ = nullptr;
    std::vector<Texture*> textures_{};
    std::vector<Texture*> subBar_{};
    std::vector<std::function<void()>> func_{}; //Bar���̏����i�[��

    bool loop_ = false;     // ���E�𒴂��鑀��������Ƃ��ɔ��΂̌��E�l�ɂ�����
    int index_ = 0;         // �I�𒆂̃o�[�ԍ�
    DirectX::XMFLOAT2 size_ = { 1,1 };  // �o�[�̍s��
    bool moveVertical_ = false;         // ��`���񎞂ȂǑI���o�[�̏㉺�ړ��s��
    bool moveHorizontal_ = false;       // ��`���񎞂ȂǑI���o�[�̍��E���E�ړ��s��
    bool sortHorizontal_ = false;       // �o�[���ׂ̗D������B���g�p�B�ʓ|����������`��ʒu�Ō떂�����Ă���
    bool islarge_ = false;  // ���͎g���ĂȂ��B�o�[�̴̪�ī���p�����ǂ��C�i�X

};

