#pragma once
/*!
* @brief	プレイヤー。
*/
#include "character/CharacterController.h"
#include "Player_BulletManager.h"
using namespace YTEngine;
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
	void PostDraw();
	void OnDestroy();
	//シャドウキャスターへの登録。
	void SetShadowCasters();

	/*!
	*@brief 左スティックのX軸の入力量を返す。
	*@return 左スティックのX軸の入力量。
	*/
	float GetlStick_x()
	{
		return lStick_x;
	}

	/*!
	*@brief 左スティックのY軸の入力量を返す。
	*@return 左スティックのY軸の入力量。
	*/
	float GetlStick_y()
	{
		return lStick_y;
	}
	/*!
	*@brief プレイヤーの前方の正規化ベクトルを返す。
	*@return プレイヤーの前方ベクトル。
	*/
	CVector3 Getforward()
	{
		return m_forward;
	}
	/*!
	*@brief	プレイヤーの座標を返す。
	*@return プレイヤーの座標。
	*/
	CVector3 Getposition()
	{
		return m_position;
	}
	/*!
	*@brief	プレイヤーの移動速度を返す。
	*@return プレイヤーの移動速度。
	*/
	CVector3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
	/*!
	*@brief ゲームカメラの前方の単位ベクトルを設定する。
	*@param[in] c_forward    カメラの前方の単位ベクトル。
	*/
	void SetCamera_forward(CVector3 c_forward)
	{
		camera_forward = c_forward;
	}

	/*!
	*@brief ゲームカメラの右方向の単位ベクトルを設定する。
	*@param[in] c_rite    カメラの右方向の単位ベクトル。
	*/
	void SetCamera_rite(CVector3 c_rite)
	{
		camera_rite = c_rite;
	}

	/*!
	*@brief ゲームカメラの右スティックの入力量を設定する。
	*@param[in] angle    カメラの右スティックの入力量(-1～1の範囲)。
	*/
	void Setcamera_rot_angle(float angle)
	{
		camera_rot_angle = angle;
	}

	/*!
	*@brief ゲームカメラの回転スピードを設定する。
	*@param[in] angle   ゲームカメラの回転スピード。
	*/
	void Setcamera_rot_speed(float angle)
	{
		camera_rot_speed = angle;
	}
	/*!
	*@brief	プレイヤーの攻撃入力を扱う。
	*/
	void Player_Bullet_Controller();
	/*!
	*@brief	プレイヤーのジャンプ関連の処理。
	*/
	void Player_Jump();
	/*
	*@brief プレイヤーの高さを返す。
	*@param[in]	height	プレイヤーの高さ。
	*/
	float Getheight()
	{
		return player_height;
	}
	/*
	*@brief プレイヤーの移動時の処理。
	*/
	void MoveEffect();
private:
	float lStick_x;                                     //左スティックの横の入力量。
	float lStick_y;                                     //左スティックの縦の入力量。
	bool player_rotationFlag = true;                    //プレイヤーが回転するかを判定するフラグ。trueで回転。
	CMatrix mRot;                                       //プレイヤーの回転行列。
	CVector3 m_moveSpeed = CVector3::Zero();            //プレイヤーの移動速度。
	CVector3 m_forward = CVector3::Zero();				//プレイヤーの前方。
	CVector3 m_rite = CVector3::Zero();                 //プレイヤーの右側。
	CQuaternion m_rotation = CQuaternion::Identity();   //キャラの回転
	CVector3 m_position = CVector3::Zero();             //プレイヤーの座標。
	SkinModel m_model;									//スキンモデル。
	CharacterController m_charaCon;                     //プレイヤーの剛体。
	CQuaternion qBias = CQuaternion::Identity();
	CVector3 camera_forward = CVector3::Zero();			//ゲームカメラの前方。
	CVector3 camera_rite = CVector3::Zero();            //ゲームカメラの右方向。
	float player_height = 100.0f;		                //プレイヤーの高さ。
	float camera_rot_angle = 0.0f;                      //ゲームカメラが回る方向を示す(-1～1の範囲)。
	float camera_rot_speed = 0.0f;                      //ゲームカメラの回転スピード。
	ID3D11ShaderResourceView* g_normalMapSRV = nullptr; //法線マップ。
	Player_BulletManager* bulletManager;
	Sprite targetSight;
	CVector3 m_targetSight_position = {0.0f,45.0f,0.0f};
};

