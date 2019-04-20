#pragma once
/*!
* @brief	プレイヤー。
*/
#include "character/CharacterController.h"
#include "Player_BulletManager.h"
#include "Player_MissileManager.h"
#include "gauge.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

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
	*@brief プレイヤーの右方の正規化ベクトルを返す。
	*@return プレイヤーの右方ベクトル。
	*/
	CVector3 Getrite()
	{
		return m_rite;
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
	*@brief	プレイヤーの横方向の移動速度を返す。
	*@return プレイヤーの横方向の移動速度。
	*/
	float GetmoveSpeed_side()
	{
		return m_moveSpeed_side;
	}
	/*!
	*@brief	プレイヤーがブーストしているかを返す。
	*@return ブーストモード時のフラグ。
	*/
	bool Getboostflag()
	{
		return MoveFlag;
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
	*@brief 射角を設定する。
	*@param[in] angle   ゲームカメラの前方の単位ベクトル。
	*/
	void Setbullet_angle(float angle)
	{
		bullet_angle = angle;
	}
	/*!
	*@brief	プレイヤーの攻撃入力を扱う。
	*/
	void Bullet_Missile_Controller();
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

	/*
	*@brief プレイヤーのエナジーの管理。
	*/
	void Energycontrol();
private:
	

	CMatrix mRot;                                       //プレイヤーの回転行列。
	CVector3 m_moveSpeed = CVector3::Zero();            //プレイヤーの移動速度。
	CVector3 m_forward = CVector3::Zero();				//プレイヤーの前方。
	CVector3 m_rite = CVector3::Zero();                 //プレイヤーの右側。
	CVector3 m_position = CVector3::Zero();             //プレイヤーの座標。
	CVector3 camera_forward = CVector3::Zero();			//ゲームカメラの前方。
	CVector3 camera_rite = CVector3::Zero();            //ゲームカメラの右方向。
	CVector3 Bullet_vector = CVector3::Zero();            //射撃方向。
	CVector3 m_targetSight_position = { 0.0f,130.0f,0.0f };//ターゲットサイト。
	SkinModel m_model;									//スキンモデル。
	CharacterController m_charaCon;                     //プレイヤーの剛体。
	CQuaternion m_rotation = CQuaternion::Identity();   //キャラの回転
	CQuaternion qBias = CQuaternion::Identity();
	ID3D11ShaderResourceView* g_normalMapSRV = nullptr; //法線マップ。
	Player_BulletManager* bulletManager;
	Sprite targetSight;
	gauge* m_gauge;
	CSoundEngine m_soundEngine;				//サウンドエンジン。
	CSoundSource m_bgm[2];					//BGM。
	CSoundSource m_se[3];					//SE。
	CSoundSource m_movese;
	CSoundSource m_Jumpse;

	float m_moveSpeed_side = 0.0f;          //プレイヤーの横方向の移動速度。
	float lStick_x;                         //左スティックの横の入力量。
	float lStick_y;                         //左スティックの縦の入力量。
	float player_height = 100.0f;		    //プレイヤーの高さ。
	float camera_rot_angle = 0.0f;          //ゲームカメラが回る方向を示す(-1～1の範囲)。
	float camera_rot_speed = 0.0f;          //ゲームカメラの回転スピード。
	float Energy;
	float bullet_angle = 0.0f;              //射角。
	float boostTime = 0.0f;                 //加速している時間。
	const float Energy_MAX = 1000.0f;
	bool EnergyOutFlag = false;             //エナジー切れを検知。
	bool MoveFlag = false;                  //加速しているかを保存。      
	bool JumpFlag = false;        
	bool player_rotationFlag = true;        //プレイヤーが回転するかを判定するフラグ。trueで回転。	                           
};

