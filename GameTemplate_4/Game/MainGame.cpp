#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Gamecamera.h"
#include "EnemyManager.h"
#include "GameBase/GameBase.h"


MainGame::MainGame()
{
}


MainGame::~MainGame()
{
	
}

void MainGame::OnDestroy()
{
	DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	DeleteGO(FindGO<Player>("Player"));
	DeleteGO(level);
	G_EnemyManager().AllDeleteEnemy();
}

bool MainGame::Start()
{
	NewGO<Level>(2, "Level");
	
	NewGO<Player>(1, "Player");
	level = FindGO<Level>("Level");
	level->Init(L"Assets/level/stage_01.tkl", nullptr);
	m_model.Init(L"Assets/modelData/sora.cmo");
	NewGO<Gamecamera>(2, "Gamecamera");
	G_EnemyManager().Start();
	G_EnemyManager().spawnenemy({ 100.0f,0.0f,100.0f });
	G_EnemyManager().spawnenemy({ 200.0f,0.0f,180.0f });
	
	return true;
}

void MainGame::Update()
{
	
	G_EnemyManager().Update();
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	if (g_pad[0].IsPress(enButtonB))
	{
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
	}
	s_x -= 0.01f;
}

void MainGame::Draw()
{
	G_EnemyManager().EnemyBulletDraw();
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}