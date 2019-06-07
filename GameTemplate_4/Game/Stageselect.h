#pragma once
using namespace YTEngine;
class Stageselect : public IGameObject
{
public:
	Stageselect();
	~Stageselect();
	bool Start();
	void Update();
	void Draw();
	void PostDraw();
private:
	Sprite Background;//背景。
	Sprite Stageselect_rogo;
	SkinModel stage00;
	SkinModel stage01;
	SkinModel stage02;

	bool L_scrollflag = false;
	bool R_scrollflag = false;
	bool lStick_stopflag = false;
	int lStick_stopcount = 0;
	float lStick_x_state = 0.0f;
	const float stage_while = 500.0f;
	int rcount = 0;
	int lcount = 0;
	int stagenum = 0;
	static const int StageNUM_MAX = 3;//ステージの合計数。
	CVector3 stage_pos[StageNUM_MAX];
	CQuaternion stage_rot[StageNUM_MAX];   //キャラの回転
	CVector3  stage_sc[StageNUM_MAX]; //キャラの拡大率。
	CVector3 stage_sc_normal= { 0.03f,0.03f,0.03f };
	CVector3 stage_sc_max = { 0.06f,0.06f,0.06f };
	float stage_y_normal = 20.0f;
	float stage_y_max = 125.0f;
	float stage_z_normal = 80.0f;
	float stage_z_max = 500.0f;
	CQuaternion qBias = CQuaternion::Identity();
};

