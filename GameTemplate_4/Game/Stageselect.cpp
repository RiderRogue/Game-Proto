#include "stdafx.h"
#include "Stageselect.h"
#include "GameBase/GameBase.h"


Stageselect::Stageselect()
{
}


Stageselect::~Stageselect()
{
}

bool Stageselect::Start()
{
	Background.Init(L"Resource/sprite/backgrounds_01.dds", 1280.0f, 720.0f);
	Background.Update(CVector3::Zero(), CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	Stageselect_rogo.Init(L"Resource/sprite/stage_select.dds",600.0f,100.0f);
	Stageselect_rogo.Update({0.0f,250.0f,0.0f}, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });

	//cmoファイルの読み込み。
	stage00.Init(L"Assets/modelData/stage_01.cmo");
	stage01.Init(L"Assets/modelData/stage_02.cmo");
	stage02.Init(L"Assets/modelData/Player_Robo.cmo");
	stage03.Init(L"Assets/modelData/stage_03.cmo");

	for (int i = 0; i <StageNUM_MAX;i++) {
		stage_pos[i].x = i * -stage_while;
		stage_pos[i].y = stage_y_normal;
		stage_pos[i].z = stage_z_normal;
		stage_rot[i] = CQuaternion::Identity();
		stage_sc[i] = stage_sc_normal;
	}
	return true;
}

void Stageselect::Update()
{
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0].GetLStickXF();

	if (stagenum<StageNUM_MAX-1) {
		if (lStick_x >= 1.0f) {
			if (L_scrollflag == false) {
				R_scrollflag = true;
				lStick_x_state = lStick_x;
			}
		}
	}
	
	if (stagenum > 0) {
		if (lStick_x <= -1.0f) {
			if (R_scrollflag == false) {
				L_scrollflag = true;
				lStick_x_state = lStick_x;
			}
		}
	}


	float sc_speed = 30.0f;
	if (R_scrollflag == true) {

		for (int i = 0; i < StageNUM_MAX; i++) {
			stage_pos[i].x += (stage_while / sc_speed);
		}
		rcount++;
		if (rcount >= sc_speed) {
			rcount = 0;
			R_scrollflag = false;
			stagenum++;
		}
	}

	else if (L_scrollflag == true) {
		for (int i = 0; i < StageNUM_MAX; i++) {
			stage_pos[i].x -= (stage_while / sc_speed);
		}
		lcount++;

		if (lcount >= sc_speed) {
			lcount = 0;
			L_scrollflag = false;
			stagenum--;
		}
	}
	Background.Update(CVector3::Zero(), CQuaternion::Identity(), { 1.0f,1.0f,1.0f });

	//ステージの回転。
	if ((R_scrollflag == false) && (L_scrollflag == false)) 
	{
		qBias.SetRotationDeg(CVector3::AxisY(), 10.0f);
		stage_rot[stagenum].Multiply(qBias);
	}
	//ステージの拡大。
	if ((R_scrollflag == false) && (L_scrollflag == false)) {
		if (stage_pos[stagenum].y < stage_y_max) {
			stage_pos[stagenum].y += 5.0f;
			if (stage_pos[stagenum].y >= stage_y_max) {
				stage_pos[stagenum].y = stage_y_max;
			}
		}
		if (stage_pos[stagenum].z < stage_z_max) {
			stage_pos[stagenum].z += 20.0f;
			if (stage_pos[stagenum].z >= stage_z_max) {
				stage_pos[stagenum].z = stage_z_max;
			}
		}
	}
	else {
		if (stage_pos[stagenum].y > stage_y_normal) {
			stage_pos[stagenum].y -= 10.0f;
			if (stage_pos[stagenum].y <= stage_y_normal) {
				stage_pos[stagenum].y = stage_y_normal;
			}
		}
		if (stage_pos[stagenum].z > stage_z_normal) {
			stage_pos[stagenum].z -= 40.0f;
			if (stage_pos[stagenum].z <= stage_z_normal) {
				stage_pos[stagenum].z = stage_z_normal;
			}
		}
	}
	//ワールド行列の更新。
	stage00.UpdateWorldMatrix(stage_pos[0], stage_rot[0], stage_sc[0]);
	stage01.UpdateWorldMatrix(stage_pos[1], stage_rot[1], stage_sc[1]);
	stage02.UpdateWorldMatrix(stage_pos[2], stage_rot[2], CVector3::One());
	stage03.UpdateWorldMatrix(stage_pos[3], stage_rot[3], stage_sc[3]);

	g_camera3D.Update();
	if ((R_scrollflag == false)&&(L_scrollflag == false)) {
		if (g_pad[0].IsTrigger(enButtonA))
		{
			GameBase* gamebase = FindGO<GameBase>("GameBase");
			gamebase->SetStage_num(stagenum);
			gamebase->ChangeScene(GameBase::GameBase_mainGame);
		}
	}
	
}

void Stageselect::Draw()
{
	Background.Draw();//背景は最初に描画する。

	
	stage00.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	stage01.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	stage02.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	stage03.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	Stageselect_rogo.Draw();
}

void Stageselect::PostDraw()
{

}