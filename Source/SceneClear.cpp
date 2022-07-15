#include "SceneClear.h"
#include "SceneManager.h"

#include "Input/Input.h"
#include "Graphics/Graphics.h"

inline void que(){
	SceneManager::Instance().changeScene(SCENE_TYPE::TITLE);
}
inline void come(){
	SceneManager::Instance().changeScene(SCENE_TYPE::GAME);
}

void SceneClear::Initialize()
{
	const DirectX::XMFLOAT2 screen = { Graphics::Instance().GetScreenWidth(),Graphics::Instance().GetScreenHeight() };
	menu_ = std::make_unique<MenuBar>(DirectX::XMFLOAT2(2, 1), false, false, false);

	menu_->SetLogo(new Texture(
		".\\Data\\Sprite\\LoadingIcon.png"
		, 1
		, { screen.x / 2,20.0f }
		, SPRITE_PIVOT::CT
	));
	/* bar */
	{
		menu_->AddSubBer(new Texture(
			".\\Data\\Sprite\\Title.png"
			, 8
			, { screen.x / 3,screen.y * 4 / 5 }
			, SPRITE_PIVOT::CM
			, { 0.25f,0.25f }
		), que);
		menu_->AddSubBer(new Texture(
			".\\Data\\Sprite\\Title.png"
			, 8
			, { screen.x * 2 / 3,screen.y * 4 / 5 }
			, SPRITE_PIVOT::CM
			, { 0.25f,0.25f }
		), come);
	}
	/* tex */
	{
		//left
		menu_->AddTexture(new Texture(
			"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
			, 8
			, { screen.x / 3,screen.y / 2 - 30.0f }
			, SPRITE_PIVOT::CB
			, { 0.1f,0.1f }
		));
		TEXTOUT.timeRender(false, 836327.38f
			, menu_->GetTexList()->back()->GetPosition()
		);

		menu_->AddTexture(new Texture(
			"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
			, 8
			, { screen.x / 3,screen.y / 2 }
			, SPRITE_PIVOT::CM
			, { 0.1f,0.1f }
		));
		menu_->AddTexture(new Texture(
			"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
			, 8
			, { screen.x / 3,screen.y / 2 + 30.0f }
			, SPRITE_PIVOT::CT
			, { 0.1f,0.1f }
		));

		//right
		menu_->AddTexture(new Texture(
			"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
			, 8
			, { screen.x * 2 / 3,screen.y / 2 - 30.0f }
			, SPRITE_PIVOT::CB
			, { 0.1f,0.1f }
		));
		menu_->AddTexture(new Texture(
			"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
			, 8
			, { screen.x * 2 / 3,screen.y / 2 }
			, SPRITE_PIVOT::CM
			, { 0.1f,0.1f }
		));
	}
	//	//right
	//	menu_->AddTexture(new Texture(
	//		"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
	//		, { screen.x * 2 / 3,screen.y / 2 - 30.0f }
	//		, SPRITE_PIVOT::CB
	//		, { 0.1f,0.1f }
	//	));
	//	menu_->AddTexture(new Texture(
	//		"C:\\Users\\2210337\\Desktop\\iPhone���\\IMG_5828.JPG"
	//		, { screen.x * 2 / 3,screen.y / 2 }
	//		, SPRITE_PIVOT::CM
	//		, { 0.1f,0.1f }
	//	));
	//}

	// ���̏������E�Đ�
	BGM = Audio::Instance().LoadAudioSource("Data/Audio/ClearBGM.wav");
	BGM->Play(false);
}

void SceneClear::Finalize()
{
	BGM.reset();
}

void SceneClear::Update(float elapsedTime)
{
	switch (state_)
	{
	case 0:
		TEXTOUT.timeRender(&elapsedTime, menu_->GetTexList()->at(1)->GetPosition());
		++state_;

	default:
		menu_->Update();
		
		break;
	}
}

void SceneClear::Render()
{
	menu_->Render();
	TEXTOUT.Render();
}

const SCENE_TYPE SceneClear::getType()
{
    return SCENE_TYPE::CLEAR;
}
