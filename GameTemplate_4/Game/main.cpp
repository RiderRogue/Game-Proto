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
	g_camera3D.SetPosition({ 0.0f, 500.0f, 800.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(50000.0f);
	
	int gameObjectPrioMax = 32;
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();

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