#include "stdafx.h"
#include "system.h"
#include "Player.h"
#include "Level.h"
#include "GameBase/GameBase.h"

using namespace YTEngine;
///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	/*Level ka;
	ka.Init(L"Assets/level/stage_02.tkl", nullptr);*/
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	int gameObjectPrioMax = 32;
	

	//�Q�[�����[�v�B
	//�G���W�����쐬���A�������B
	if (Engine().Init(gameObjectPrioMax) == true) {
		//GameBase���쐬�B
		NewGO<GameBase>(2, "GameBase");

		//�Q�[�����[�v�����s�B
		Engine().RunGameLoop();
	}
	//�G���W���̏I�������B
	Engine().Final();
}