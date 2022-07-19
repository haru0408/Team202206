#include "BaseScene.h"

#include <list>

#include "Misc.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"

static std::vector<std::shared_ptr<Sprite_batch>> sprs_{};

Sprite_batch::Sprite_batch(const char* filename,size_t max_sprites) :Sprite(filename) 
{
    filename_ = filename; 
    max_vertexCnt = 6 * max_sprites;

    /* 頂点バッファobj生成 */
    auto hr{ S_OK };
    std::unique_ptr<Vertex[]> vertices{ std::make_unique<Vertex[]>(max_vertexCnt) };
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(Vertex) * max_vertexCnt;   //buffer size(byte)
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices.get();  //初期化データへのポインタ
    subresource_data.SysMemPitch = 0;   //テクスチャの1行の戦闘から次の行までの距離(byte)
    subresource_data.SysMemSlicePitch = 0;  //深度レベルの開始から次レベルまでの距離(byte)
    hr = Graphics::Instance().GetDevice()->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}


void Sprite_batch::begin(ID3D11DeviceContext* immediate_context)
{
    vertices.clear();
    immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);
    immediate_context->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
    immediate_context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
    immediate_context->RSSetState(rasterizerState.Get());
}
void Sprite_batch::end(ID3D11DeviceContext* immediate_context)
{
    HRESULT hr{ S_OK };

    /* 頂点バッファobj更新 */
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(
        vertexBuffer.Get()
        , 0
        , D3D11_MAP_WRITE_DISCARD
        , 0
        , &mapped_subresource
    );  //サブリソースへのGPUアクセス拒否
    //書き込み用のマップ済みサブリソースは仕様しないこと
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    size_t vertex_count = vertices.size();
    Vertex* data{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
    if (data != nullptr)
    {
        const Vertex* p = vertices.data();
        memcpy_s(data, max_vertexCnt *sizeof(Vertex), p, vertex_count * sizeof(Vertex));
    }

    immediate_context->Unmap(vertexBuffer.Get(), 0);


    /* 頂点バッファバインド */
    UINT stride{ sizeof(Vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);


    /* 入力レイアウトobjバインド */
    immediate_context->IASetInputLayout(inputLayout.Get());

    /* primitiveタイプ及びデータ順序情報のバインド */
    //immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    /* primitiv描画 */
    immediate_context->Draw(static_cast<UINT>(vertex_count), 0);
}
void Sprite_batch::Render(ID3D11DeviceContext* immediate_context,
    float dx, float dy,
    float dw, float dh,
    float sx, float sy,
    float sw, float sh,
    float angle,
    float r, float g, float b, float a)
{
    // 現在設定されているビューポートからスクリーンサイズを取得する。
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    immediate_context->RSGetViewports(&numViewports, &viewport);
    float screen_width = viewport.Width;
    float screen_height = viewport.Height;

    // スプライトを構成する４頂点のスクリーン座標を計算する
    DirectX::XMFLOAT2 positions[] = {
        DirectX::XMFLOAT2(dx,      dy),			// 左上
        DirectX::XMFLOAT2(dx + dw, dy),			// 右上
        DirectX::XMFLOAT2(dx,      dy + dh),	// 左下
        DirectX::XMFLOAT2(dx + dw, dy + dh),	// 右下
    };

    // スプライトを構成する４頂点のテクスチャ座標を計算する
    DirectX::XMFLOAT2 texcoords[] = {
        DirectX::XMFLOAT2(sx,      sy),			// 左上
        DirectX::XMFLOAT2(sx + sw, sy),			// 右上
        DirectX::XMFLOAT2(sx,      sy + sh),	// 左下
        DirectX::XMFLOAT2(sx + sw, sy + sh),	// 右下
    };

    // スプライトの中心で回転させるために４頂点の中心位置が
    // 原点(0, 0)になるように一旦頂点を移動させる。
    float mx = dx + dw * 0.5f;
    float my = dy + dh * 0.5f;
    for (auto& p : positions)
    {
        p.x -= mx;
        p.y -= my;
    }

    // 頂点を回転させる
    const float PI = 3.141592653589793f;
    float theta = angle * (PI / 180.0f);	// 角度をラジアン(θ)に変換
    float c = cosf(theta);
    float s = sinf(theta);
    for (auto& p : positions)
    {
        DirectX::XMFLOAT2 r = p;
        p.x = c * r.x + -s * r.y;
        p.y = s * r.x + c * r.y;
    }

    // 回転のために移動させた頂点を元の位置に戻す
    for (auto& p : positions)
    {
        p.x += mx;
        p.y += my;
    }

    // スクリーン座標系からNDC座標系へ変換する。
    for (auto& p : positions)
    {
        p.x = 2.0f * p.x / screen_width - 1.0f;
        p.y = 1.0f - 2.0f * p.y / screen_height;
    }

    // UV正規化
    for (auto& it : texcoords)
    {
        it.x /= textureWidth;
        it.y /= textureHeight;
    }

    // vectorへ挿入
    {
        vertices.emplace_back(DirectX::XMFLOAT3(positions[0].x, positions[0].y, 0), DirectX::XMFLOAT4(r, g, b, a), texcoords[0]);
        vertices.emplace_back(DirectX::XMFLOAT3(positions[1].x, positions[1].y, 0), DirectX::XMFLOAT4(r, g, b, a), DirectX::XMFLOAT2(texcoords[3].x, texcoords[0].y));
        vertices.emplace_back(DirectX::XMFLOAT3(positions[2].x, positions[2].y, 0), DirectX::XMFLOAT4(r, g, b, a), DirectX::XMFLOAT2(texcoords[0].x, texcoords[3].y));
        vertices.emplace_back(DirectX::XMFLOAT3(positions[2].x, positions[2].y, 0), DirectX::XMFLOAT4(r, g, b, a), DirectX::XMFLOAT2(texcoords[0].x, texcoords[3].y));
        vertices.emplace_back(DirectX::XMFLOAT3(positions[1].x, positions[1].y, 0), DirectX::XMFLOAT4(r, g, b, a), DirectX::XMFLOAT2(texcoords[3].x, texcoords[0].y));
        vertices.emplace_back(DirectX::XMFLOAT3(positions[3].x, positions[3].y, 0), DirectX::XMFLOAT4(r, g, b, a), texcoords[3]);
        return;
    }
}

Texture::Texture(const char* filename,size_t maxCnt, const DirectX::XMFLOAT2& pos, SPRITE_PIVOT pivot, const DirectX::XMFLOAT2& scale, int zCode, const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle, const DirectX::XMFLOAT4& color)
{
    std::shared_ptr<Sprite_batch> spr = nullptr;
    for (auto& it : sprs_)
    {
        if (it->GetFilename()==filename)
        {
            spr = it;
            break;
        }
    }
    if (spr == nullptr) {
        spr = std::make_shared<Sprite_batch>(filename, maxCnt);
        sprs_.emplace_back(spr);
    }
    file_ = spr;

    position_ = pos;
    pivot_ = pivot;
    scale_ = scale;
    texPos_ = texPos;
    if (texSize.x * texSize.y == 0.0f)
        texSize_ = { static_cast<float>(file_->GetTextureWidth()),static_cast<float>(file_->GetTextureHeight())};
    else
        texSize_ = texSize;
    angle_ = angle;
    color_ = color;
}
void Texture::Render(const DirectX::XMFLOAT2& p)
{
    auto dc = Graphics::Instance().GetDeviceContext();

    auto pos = GetPosition();
    switch (pivot_)
    {
    case SPRITE_PIVOT::LT: /*nothing*/ break;
    case SPRITE_PIVOT::CT: pos.x -= texSize_.x * scale_.x / 2;                                      break;
    case SPRITE_PIVOT::RT: pos.x -= texSize_.x * scale_.x;                                          break;
    case SPRITE_PIVOT::LM:                                      pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::CM: pos.x -= texSize_.x * scale_.x / 2,  pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::RM: pos.x -= texSize_.x * scale_.x,      pos.y -= texSize_.y * scale_.y / 2; break;
    case SPRITE_PIVOT::LB:                                      pos.y -= texSize_.y * scale_.y;     break;
    case SPRITE_PIVOT::CB: pos.x -= texSize_.x * scale_.x / 2,  pos.y -= texSize_.y * scale_.y;     break;
    case SPRITE_PIVOT::RB: pos.x -= texSize_.x * scale_.x,      pos.y -= texSize_.y * scale_.y;     break;
    }
    pos.x += p.x, pos.y += p.y;

    file_->Render(dc
        , pos.x, pos.y
        , texSize_.x * scale_.x, texSize_.y * scale_.y
        , texPos_.x, texPos_.y
        , texSize_.x, texSize_.y
        , angle_
        , color_.x, color_.y, color_.z, color_.w
    );
}

//---------------------------------//



//template<typename T>
//void textout(T* text
//    , DirectX::XMFLOAT2 pos = {}
//    , DirectX::XMFLOAT2 scale = { 1,1 }
//    , SPRITE_PIVOT pivot = SPRITE_PIVOT::LT
//);

void TextInput::numRender(int num, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 pos)
{
    for (; num != 0; num /= 10)
    {
        text_.emplace_back(std::make_shared<Texture>(
            numFont.filename,256
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' + num % 10), numFont.fontSize.y * static_cast<int>('0') / 16)
            , numFont.fontSize
        ));
    }
}
void TextInput::timeRender(float* time
    , DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale
    , int decimalP)
{
    timers_.emplace_back(time, pos, scale, decimalP);
}
void TextInput::timeRender(bool update,float time
    , DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale
    , int decimalP)
{
    DirectX::XMFLOAT3 result{}; result.z = time;

    for (; 60.0f <= result.z; result.z -= 60.0f, ++result.y);
    for (; 60.0f <= result.y; result.y -= 60.0f, ++result.x);
    for (; 24.0f <= result.x; result.x -= 24.0f);

    timeRender(update,result, scale, pos, decimalP);
}
void TextInput::timeRender(bool update,DirectX::XMFLOAT3 time
    , DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale
    , int decimalP)
{
    timeRender(update,static_cast<int>(time.x), static_cast<int>(time.y), time.z
        , scale, pos, decimalP
    );
}
void TextInput::timeRender(bool update,int hour, int minutes, float seconds
    , DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale
    , int decimalP)
{
    if (60 <= seconds) { seconds -= 60, ++minutes; }
    if (60 <= minutes) { minutes -= 60, ++hour; }
    if (24 <= hour) { hour -= 24; }

    std::vector<char>pue;
    {
        pue.emplace_back('0' + static_cast<char>(hour / 10));
        pue.emplace_back('0' + static_cast<char>(hour % 10));
        pue.emplace_back(':');
        pue.emplace_back('0' + static_cast<char>(minutes / 10));
        pue.emplace_back('0' + static_cast<char>(minutes % 10));
        pue.emplace_back(':');
        pue.emplace_back('0' + static_cast<char>(static_cast<int>(seconds) / 10.0f));
        pue.emplace_back('0' + static_cast<char>(static_cast<int>(seconds) % 10));
        seconds -= static_cast<int>(seconds) / 10 * 10;
        seconds -= static_cast<int>(seconds) % 10;
        if (0 < decimalP) pue.emplace_back('.');
        while (0 < decimalP)
        {
            seconds *= 10.0f;
            pue.emplace_back('0' + static_cast<char>(static_cast<int>(seconds) % 10));
            seconds -= static_cast<int>(seconds) % 10;
            --decimalP;
        }
    }

    DirectX::XMFLOAT2 fontIndex = { 
         numFont.sprSize.x / numFont.fontSize.x
        ,numFont.sprSize.y / numFont.fontSize.y
    };
    if (update)
    {
        for (auto& it:pue)
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (it % static_cast<int>(fontIndex.x)), numFont.fontSize.y * static_cast<int>(it / fontIndex.x))
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
    }
    else
    {
        for (auto& it : pue)
        {
            text_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (it % static_cast<int>(fontIndex.x)), numFont.fontSize.y * static_cast<int>(it / fontIndex.x))
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
    }
    return;

    if(update)
    {
        if (hour < 10)
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;

            if (hour == 0)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * ('0'), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x += numFont.fontSize.x;
            }
        }
        else {
            for (pos.x += numFont.fontSize.x; hour != 0; hour /= 10)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + hour % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x -= numFont.fontSize.x;
            }pos.x += numFont.fontSize.x * 3;
        }
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                , numFont.fontSize
                ));
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , DirectX::XMFLOAT2(pos.x, pos.y - numFont.fontSize.y / 3)
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
        if (minutes < 10)
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;

            if (minutes == 0)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x += numFont.fontSize.x;
            }
        }
        else {
            for (pos.x += numFont.fontSize.x; minutes != 0; minutes /= 10)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + minutes % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x -= numFont.fontSize.x;
            }pos.x += numFont.fontSize.x * 3;
        }
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                , numFont.fontSize
                ));
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , DirectX::XMFLOAT2(pos.x, pos.y - numFont.fontSize.y / 3)
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
        if (seconds < 10.0f)
        {
            updates_.emplace_back(std::make_unique<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;

            if (seconds < 1.0f)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * (('0'+static_cast<int>(seconds)%10 )% 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x += numFont.fontSize.x;
            }
        }
        else {
            pos.x += numFont.fontSize.x;
            for (int s = static_cast<int>(seconds); s != 0; s /= 10, seconds /= 10.0f)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + static_cast<int>(s) % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                pos.x -= numFont.fontSize.x;
            }pos.x += numFont.fontSize.x * 3;
        }
        if (0 < decimalP)
        {
            {
                float nextPos = numFont.fontSize.x / 3;
                pos.x -= nextPos;
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                    , numFont.fontSize
                    ));
                pos.x += nextPos;
            }
            pos.x += numFont.fontSize.x;
            for (int i = 0; i < decimalP; ++i, seconds /= 10.0f)
            {
                updates_.emplace_back(std::make_unique<Texture>(
                    numFont.filename, 64
                    , pos
                    , SPRITE_PIVOT::LT
                    , scale
                    , 0
                    , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + static_cast<int>(seconds) % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                    , numFont.fontSize
                    ));
                if (i == 0)
                    pos.x -= numFont.fontSize.x;
                else if (i == 1)
                    pos.x += numFont.fontSize.x * 2;
                else pos.x += numFont.fontSize.x;
            }
        }
    }
    else
    {
    if (hour < 10)
    {
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * ('0'%16), numFont.fontSize.y * static_cast<int>('0') / 16)
            , numFont.fontSize
            ));
        pos.x += numFont.fontSize.x;

        if (hour == 0)
        {
            text_.emplace_back(std::make_shared<Texture>(
                            numFont.filename,64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0'), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
    }
    else {
        for (pos.x += numFont.fontSize.x; hour != 0; hour /= 10)
        {
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + hour % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x -= numFont.fontSize.x;
        }pos.x += numFont.fontSize.x * 3;
    }
    {
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
            , numFont.fontSize
        ));
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , DirectX::XMFLOAT2(pos.x, pos.y - numFont.fontSize.y / 3)
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
            , numFont.fontSize
        ));
        pos.x += numFont.fontSize.x;
    }
    if (minutes < 10)
    {
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
            , numFont.fontSize
            ));
        pos.x += numFont.fontSize.x;

        if (minutes == 0)
        {
            text_.emplace_back(std::make_shared<Texture>(
                            numFont.filename,64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
    }
    else {
        for (pos.x += numFont.fontSize.x; minutes != 0; minutes /= 10)
        {
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + minutes % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x -= numFont.fontSize.x;
        }pos.x += numFont.fontSize.x * 3;
    }
    {
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
            , numFont.fontSize
        ));
        text_.emplace_back(std::make_shared<Texture>(
                        numFont.filename,64
            , DirectX::XMFLOAT2(pos.x, pos.y - numFont.fontSize.y / 3)
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
            , numFont.fontSize
        ));
        pos.x += numFont.fontSize.x;
    }
    if (seconds < 10.0f)
    {
        text_.emplace_back(std::make_shared<Texture>(
            numFont.filename, 64
            , pos
            , SPRITE_PIVOT::LT
            , scale
            , 0
            , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
            , numFont.fontSize
            ));
        pos.x += numFont.fontSize.x;

        if (seconds < 1.0f)
        {
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * ('0' % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x += numFont.fontSize.x;
        }
    }
    else {
        pos.x += numFont.fontSize.x;
        for (int s = static_cast<int>(seconds); s != 0; s /= 10, seconds /= 10.0f)
        {
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + static_cast<int>(s) % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            pos.x -= numFont.fontSize.x;
        }pos.x += numFont.fontSize.x * 3;
    }
    if (0 < decimalP)
    {
        {
            float nextPos = numFont.fontSize.x / 3;
            pos.x -= nextPos;
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * 14, numFont.fontSize.y * 2)
                , numFont.fontSize
                ));
            pos.x += nextPos;
        }
        pos.x += numFont.fontSize.x;
        for (int i = 0; i < decimalP; ++i, seconds /= 10.0f)
        {
            text_.emplace_back(std::make_shared<Texture>(
                numFont.filename, 64
                , pos
                , SPRITE_PIVOT::LT
                , scale
                , 0
                , DirectX::XMFLOAT2(numFont.fontSize.x * (('0' + static_cast<int>(seconds) % 10) % 16), numFont.fontSize.y * static_cast<int>('0') / 16)
                , numFont.fontSize
                ));
            if (i == 0)
                pos.x -= numFont.fontSize.x;
            else if (i == 1)
                pos.x += numFont.fontSize.x * 2;
            else pos.x += numFont.fontSize.x;
        }
    }
    }
}

void TextInput::Render()
{
    
    text_.begin()->get()->GetFile()->begin(Graphics::Instance().GetDeviceContext());
    for (auto& it : text_)
        it->Render({});
    text_.begin()->get()->GetFile()->end(Graphics::Instance().GetDeviceContext());

    for (auto& it : timers_)
    {
        //it.timer_ += *it.addTime_;
        timeRender(true, (*it.addTime_), it.position_, it.scale_, it.decimalP_);
    }
    updates_.begin()->get()->GetFile()->begin(Graphics::Instance().GetDeviceContext());
    for (auto it = updates_.begin(); it != updates_.end(); )
    {
        if (it->get() != nullptr)
        {
            it->get()->Render({});
            if (it == updates_.end()-1)
            {
                it->get()->GetFile()->end(Graphics::Instance().GetDeviceContext());
                it = updates_.erase(it);
                break;
            }
            if (it->get()->GetFile() != (it + 1)->get()->GetFile())
            {
                it->get()->GetFile()->end(Graphics::Instance().GetDeviceContext());
                    (it + 1)->get()->GetFile()->begin(Graphics::Instance().GetDeviceContext());
            }
            it = updates_.erase(it);
            //continue;
        }
    };
    updates_.clear();
}


//---------------------------------//

void MenuBar::Update() {
    // change scene
    {
        auto& input = Input::Instance().GetGamePad();
        int barSize = static_cast<int>(subBar_.size());

        //Bar選択
        //AXIS LX
        {
            static bool BarLX_up = false;
            const auto& axis = input.GetAxisLX();

            if (BarLX_up == false) //trigger
                if (axis != 0.0f)
                {
                    int old = index_;

                    //増減
                    if (axis <= -0.75f)
                        --index_;
                    else if (0.75f <= axis)
                        ++index_;

                    //左右限界うんぬん
                    if (moveHorizontal_ == false) {
                        int sx = static_cast<int>(size_.x);
                        if (old / sx != index_ / sx)
                            index_ = old;
                    }
                    //clamp制御
                    if (loop_)
                    {
                        if (index_ < 0) index_ = barSize - 1;
                        if (barSize - 1 < index_) index_ = 0;
                    }
                    else index_ = (std::min)((std::max)(0, index_), static_cast<int>(barSize) - 1);
                }
            BarLX_up = (axis != 0.0f);
        }
#if 1
        //AXIS LY
        {
            static bool BarLY_up = false;
            const auto& axis = input.GetAxisLY();

            if (BarLY_up == false) //trigger
                if (axis != 0.0f)
                {
                    //増減
                    if (axis <= -0.75f)
                        //--index_;
                        index_ = (moveVertical_)
                        ? index_ - size_.x
                        : index_--;
                    else if (0.75f <= axis)
                        index_ = (moveVertical_)
                        ? index_ + size_.x
                        : index_++;

                    //clamp制御
                    if (loop_)
                    {
                        if (index_ < 0) index_ = barSize - 1;
                        if (barSize - 1 < index_) index_ = 0;
                    }
                    else index_ = (std::min)((std::max)(0, index_), static_cast<int>(barSize) - 1);
                }
            BarLY_up = (axis != 0.0f);
        }
#else
        //BUTTON
        {
            switch ((input.GetButtonDown() & (GamePad::BTN_UP | GamePad::BTN_DOWN)))
            {
            case (GamePad::BTN_UP):
                --index_;
                if (index_ < 0)
                    index_ = (loop_) ? barSize - 1 : 0; //loopオンなら一番下にする。オフなら最高値にする。
                break;
            case (GamePad::BTN_DOWN):
                ++index_;
                if (barSize <= index_)
                    index_ = (loop_) ? 0 : barSize - 1; //loopオンなら一番上にする。
                break;
            }
        }
#endif

        //change
        if (Input::Instance().GetGamePad().GetButtonDown()
            & GamePad::BTN_START)
            func_[index_]();
    }

    // bar update
    {
        //はいぷろ見ながらつくる
        //subBar_[index_]->SetScale()
        for (auto& bar : subBar_)
        {
            bar->SetColor({ 0.70f,0.70f,0.70f,1 });
            if (bar == subBar_.at(index_)) bar->SetColor({ 1,1,1,1 });
        }
    }
}

void MenuBar::Render()
{
    auto dc = Graphics::Instance().GetDeviceContext();

#if 1
    // BG
    if (BG_ != nullptr)
    {
        BG_->GetFile()->begin(dc);
        BG_->Render(position_);
        BG_->GetFile()->end(dc);
    }

    // texture
    if (!textures_.empty())
    {
        textures_.front()->GetFile()->begin(dc);
        for (auto& it : textures_)
        {
            it->Render(position_);
            if (it == textures_.back()
                || (it)->GetFile() != (it + 1)->GetFile())
            {
                it->GetFile()->end(dc);
                it->GetFile()->begin(dc);
            }
        }
        textures_.back()->GetFile()->end(dc);
    }

    // subBar_
    if (!subBar_.empty())
    {
        subBar_.front()->GetFile()->begin(dc);
        for (auto& it : subBar_)
        {
            it->Render(position_);
            if (it == textures_.back()
                || (it)->GetFile() != (it + 1)->GetFile())
            {
                it->GetFile()->end(dc);
                it->GetFile()->begin(dc);
            }
        }
        subBar_.back()->GetFile()->end(dc);
    }

    // titleLogo_
    if (logo_ != nullptr)
    {
        logo_->GetFile()->begin(dc);
        logo_->Render(position_);
        logo_->GetFile()->end(dc);
    }
#else
    std::list<Texture, int> spr{};

    spr.emplace_back(logo_, logo_->GetZ());
    spr.emplace_back(BG_,BG_->GetZ());
    for (auto& it : textures_)
        spr.emplace_back(it,it->GetZ());
    for (auto& it : subBar_)
        spr.emplace_back(it,it->GetZ());

    spr.sort();
    for (auto& it : spr)
        it.Render(position_);
#endif


}

//---------------------------------//

