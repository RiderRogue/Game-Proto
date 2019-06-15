#include "stdafx.h"
#include "Stage_03.h"
#include "Player.h"
#include "Gamecamera.h"
#include "EnemyManager.h"



Stage_03::Stage_03()
{
}


Stage_03::~Stage_03()
{
}

void Stage_03::OnDestroy()
{
	DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	DeleteGO(FindGO<Player>("Player"));
	DeleteGO(level);
	DeleteGO(boss);
	G_EnemyManager().AllDeleteEnemy();
}

bool Stage_03::Start()
{
	level = NewGO<Level>(2, "Level");

	level->Init(L"Assets/level/stage_03.tkl", [&](LevelObjectData data)
	{
		auto name = data.name;
		auto No = wcscmp(name, (L"Player_Robo"));
		if (No == 0) {
			//プレイヤーの座標を取得。
			FindGO<Player>("Player")->Setposition(data.position);
			return true;
		}
		else if (0 == wcscmp(name, (L"E_PosR"))) {
			E_PosR = data.position;
			return true;
		}
		else if (0 == wcscmp(name, (L"E_PosL"))) {
			E_PosL = data.position;
			return true;
		}
		else if (0 == wcscmp(name, (L"E_PosFront"))) {
			E_PosFront = data.position;
			return true;
		}
		else if (0 == wcscmp(name, (L"E_PosBack"))) {
			E_PosBack = data.position;
			return true;
		}
		return false;
	}
	);
	m_model.Init(L"Assets/modelData/sora.cmo");
	m_model.SetIsspecPow(10000.0f);
	NewGO<Gamecamera>(2, "Gamecamera");
	boss = NewGO<EnemyBoss_Launcher>(1, "EnemyBoss_Launcher");
	boss->SetPosition(E_PosFront);
	boss->SetActionRange(E_PosR, E_PosL, E_PosFront, E_PosBack);

	G_EnemyManager().Start();

	return true;
}

void Stage_03::Update()
{
	G_EnemyManager().Update();
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}

void Stage_03::Draw()
{
	G_EnemyManager().EnemyBulletDraw();
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}