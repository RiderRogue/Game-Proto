#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
//#include "Gamecamera.h"
//#include "EnemyManager.h"
#include "GameBase/GameBase.h"
#include "graphics/EffekseerManager.h"
#include "Stage_00.h"
#include "Stage_01.h"


MainGame::MainGame()
{
}


MainGame::~MainGame()
{
	
}

void MainGame::OnDestroy()
{
	//DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	//DeleteGO(FindGO<Player>("Player"));
	//DeleteGO(level);
	//G_EnemyManager().AllDeleteEnemy();
	G_EffekseerManager().OnDestroy();
	FlyEnemylist.erase(FlyEnemylist.begin(), FlyEnemylist.end());
}

bool MainGame::Start()
{
	stage_num= FindGO<GameBase>("GameBase")->GetStage_num();
	Player_desFlag = false;
	NewGO<Player>(1, "Player");
	switch (stage_num)
	{
	case 0:
		NewGO<Stage_00>(1, "Stage_00");
		break;
	case 1:
		NewGO<Stage_01>(1, "Stage_01");
		break;
	case 2:
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
		break;
	default:
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
		break;
	}
	//NewGO<Player>(1, "Player");
	//level = NewGO<Level>(2, "Level");
	//level->Init(L"Assets/level/stage_02.tkl", [&](LevelObjectData data)
	//{
	//	auto name = data.name;
	//	auto No = wcscmp(name, (L"Player_Robo"));
	//	if (No == 0) {
	//		//プレイヤーの座標を取得。
	//		FindGO<Player>("Player")->Setposition(data.position);
	//		return true;
	//	}
	//	else if (0== wcscmp(name, (L"FlyEnemy"))) {
	//		//敵の座標を取得。
	//		FlyEnemylist.push_back(data.position);
	//		return true;
	//	}
	//	return false;
	//}
	//);
	//m_model.Init(L"Assets/modelData/sora.cmo");
	//m_model.SetIsspecPow(10000.0f);
	//
	//G_EnemyManager().Start();

	//for (CVector3 flyE_pos: FlyEnemylist) {
	//	G_EnemyManager().spawnenemy(flyE_pos);
	//}
	//G_EnemyManager().spawnenemy({ 100.0f,0.0f,100.0f });
	//G_EnemyManager().spawnenemy({ 200.0f,0.0f,180.0f });
	
	//NewGO<Gamecamera>(2, "Gamecamera");
	return true;
}

void MainGame::Update()
{
	G_EffekseerManager().Update();
	//G_EnemyManager().Update();
	//ワールド行列の更新。
	//m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	if (g_pad[0].IsPress(enButtonB)||(Player_desFlag==true))
	{
		switch (stage_num)
		{
		case 0:
			DeleteGO(FindGO<Stage_00>("Stage_00"));
			FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
			break;
		case 1:
			DeleteGO(FindGO<Stage_01>("Stage_01"));
			FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
			break;
		case 2:
			FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
			break;
		default:
			FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
			break;
		}
		//FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_title);
		//DeleteGO(this);
	}
}

void MainGame::Draw()
{
	//G_EnemyManager().EnemyBulletDraw();
	/*m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);*/
}