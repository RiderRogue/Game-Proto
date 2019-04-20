#pragma once

#include "Player.h"
class Gamecamera : public IGameObject
{
public:
	Gamecamera();
	~Gamecamera();
	bool Start();
	void Update();

	/*!
	*@brief プレイヤー座標を取得する。
	*@param[in] Ppos    Player_positioに設定するプレイヤーの座標。
	*/
	void GetPlayer_position(CVector3 Ppos)
	{
		Player_position = Ppos;
	}

	/*!
	*@brief ゲームカメラ座標を設定する。
	*@param[in] Cpos    カメラに設定する座標。
	*/
	void SetCamera_position(CVector3 Cpos)
	{
		Camera_vector = Cpos;
	}

	/*!
	*@brief プレイヤーの前方を示す単位ベクトルから、プレイヤーの後ろ向きの単位ベクトルを取得する。
	*/
	CVector3 SetPlayer_Cameravector()
	{
		CVector3 P_CVector;
		P_CVector = player->Getforward();
		P_CVector *= -1;
		return P_CVector;
	}

	/*!
	*@brief ゲームカメラの前方の方向ベクトルを返す。
	*@return ゲームカメラの前方の方向ベクトル。
	*/
	CVector3 Getforward()
	{
		return m_forward;
	}

	/*!
	*@brief ゲームカメラの右方向の方向ベクトルを返す。
	*@return ゲームカメラの右方向の方向ベクトル。
	*/
	CVector3 Getrite()
	{
		return m_rite;
	}
	/*!
	*@brief 回転の軸にプレイヤーの座標を設定する。
	*/
	void qBiasSet_Playerposition()
	{
		CVector3 setposition = player->Getposition();
		qBias.x = setposition.x;
		qBias.y = setposition.y;
		qBias.z = setposition.z;
	}
	/*!
	*@brief ゲームカメラの初期化。
	*/
	void Init();
private:
	const float Camera_Length_MAX = 200.0f;
	const float Camera_Length_MIN = 200.0f;
	float Camera_Length = Camera_Length_MAX;
	float Player_moveside = 0.0f;
	float rStick_x;                                     //右スティックの横の入力量。
	float rStick_y;                                     //右スティックの縦の入力量。
	float lStick_x;                                     //左スティックの横の入力量。
	float lStick_y;                                     //左スティックの縦の入力量。
	CVector3 Player_position = CVector3::Zero();        //プレイヤーの座標を格納する。
	CVector3 Camera_vector = CVector3::Zero();          //カメラの座標を代入。
	Player* player;                                     //プレイヤーの座標を取得するために使う。
	CMatrix mRot;                                       //カメラの回転行列。
	CVector3 m_forward = CVector3::Zero();				//カメラの前方。
	CVector3 m_rite = CVector3::Zero();                 //カメラの右方向。
	CVector3 Player_Cameravector = CVector3::Zero();    //プレイヤーからカメラへの単位ベクトル(方向ベクトル)。
	CQuaternion m_rotation = CQuaternion::Identity();   //カメラの任意軸回転。
	CQuaternion qBias = CQuaternion::Identity();
	float rotation_speed;                               //カメラの回転スピード。
	bool boostflag = false;                             //ブースト時のフラグ。
};