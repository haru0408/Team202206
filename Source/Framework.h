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
*	なんか:		SceneGameの静的変数の削除及びそれの関数呼び出し削除。
*				上記の関数呼び出しをSceneManagerに変更。
*  } - 2022年6月29日16:03
* Fuji {
*	なんか:		SceneManager の Initialize を使おうとしたけどやっぱりやめた。
*  } - 2022年7月1日10:38
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

