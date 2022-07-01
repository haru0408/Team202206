#pragma once

#include <windows.h>
#include "HighResolutionTimer.h"
#include "Audio/Audio.h"
#include "Graphics/Graphics.h"
#include "Input/input.h"
/*
* Log:
* Fuji {
*	#uninclude:	BaseScene(.cpp)
*	#include:	SceneManager(.cpp)
*	�Ȃ�:		SceneGame�̐ÓI�ϐ��̍폜�y�т���̊֐��Ăяo���폜�B
*				��L�̊֐��Ăяo����SceneManager�ɕύX�B
*  } - 2022�N6��29��16:03
* Fuji {
*	�Ȃ�:		SceneManager �� Initialize ���g�����Ƃ������ǂ���ς��߂��B
*  } - 2022�N7��1��10:38
*/


class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

private:
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Render(float elapsedTime/*Elapsed seconds from last frame*/);

	void CalculateFrameStats();

public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const HWND				hWnd;
	HighResolutionTimer		timer;
	Audio					audio;
	Graphics				graphics;
	Input					input;
};

