#include "stdafx.h"
#include "Stage_00.h"
#include "Player.h"
#include "Gamecamera.h"
#include "EnemyManager.h"


Stage_00::Stage_00()
{
}


Stage_00::~Stage_00()
{
}

void Stage_00::OnDestroy()
{
	DeleteGO(FindGO<Gamecamera>("Gamecamera"));
	DeleteGO(FindGO<Player>("Player"));
	DeleteGO(level);
	G_EnemyManager().AllDeleteEnemy();
}
bool Stage_00::Start()
{
	level = NewGO<Level>(2, "Level");
	
	level->Init(L"Assets/level/stage_00.tkl", [&](LevelObjectData data)
	{
		auto name = data.name;
		auto No = wcscmp(name, (L"Player_Robo"));
		if (No == 0) {
			//プレイヤーの座標を取得。
			FindGO<Player>("Player")->Setposition(data.position);
			return true;
		}
		else if (0 == wcscmp(name, (L"FlyEnemy"))) {
			//敵の座標を取得。
			FlyEnemylist.push_back(data.position);
			return true;
		}
		return false;
	}
	);
	m_model.Init(L"Assets/modelData/sora.cmo");
	m_model.SetIsspecPow(10000.0f);
	NewGO<Gamecamera>(2, "Gamecamera");
	G_EnemyManager().Start();

	for (CVector3 flyE_pos : FlyEnemylist) {
		G_EnemyManager().spawnenemy(flyE_pos);
	}
	
	return true;
}

void Stage_00::Update()
{
	G_EnemyManager().Update();
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}

void Stage_00::Draw()
{
	G_EnemyManager().EnemyBulletDraw();
	m_model.Draw(
	enRenderMode_Normal,
	g_camera3D.GetViewMatrix(),
	g_camera3D.GetProjectionMatrix()
	);
}