#pragma once
/*!
* @brief	エネミー(敵)。
*/
#include"Player.h"
#include "character/EnemyController.h"

using namespace YTEngine;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual~Enemy();
	virtual bool Start();
	virtual void Update();
	virtual void Draw();
	virtual void PostDraw();
	//サーチ状態での更新。
	virtual void SearchUpdate();
	//攻撃状態での更新。
	virtual void AttackUpdate();
	enum EnemyState
	{
		Enemy_Search,
		Enemy_Attack,
		Enemy_Num
	};
	//プレイヤーのサーチ。
	virtual void SearchPlayer();
	//シャドウキャスターへの登録。
	void SetShadowCasters();
	/*!
	* @brief	ダメージ＆死亡フラグ。
	*@param[in]	damage		受けたダメージ量。
	*/
	virtual void enemyDamage(int damage);

	virtual void enemyDamage_Blackhole(int damage,CVector3 b_position);
	/*!
	*@brief	死亡フラグの取得。
	*/
	bool Getenemy_deathflag()
	{
		return enemy_deathflag;
	}
	/*!
	*@brief	座標の設定。
	*/
	virtual void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(position);
		m_position_center = position;
		m_position_center.y += (enemy_height / 2);
	}
	/*!
	*@brief	当たり判定などで扱う、中心座標の取得。
	*/
	virtual CVector3 GetPosition_center()
	{
		return m_position_center;
	}
	/*!
	*@brief	EnemyManagerでの配列番号の保存。
	*/
	virtual void SetslotNumber(int slotNumber)
	{
		My_slotNumber = slotNumber;
	}

	//敵弾の発射処理。
	virtual void EnemyShot();
	//プレイヤー方向へ回転。
	void PlayertoRotation();
	//2つのベクトルの角度を角度表記(degree)で返す。
	float VectorAngleDeg(CVector3 c);
protected:
	CMatrix mRot;                                       //敵の回転行列。
	CVector3 vDist;                                     //作業用。	
	CVector3 m_moveSpeed = CVector3::Zero();            //敵の移動速度。
	CVector3 m_premoveSpeed = CVector3::Zero();         //クラス外からの敵の移動速度。
	CVector3 m_forward = CVector3::Zero();				//敵の前方。
	CVector3 m_rite = CVector3::Zero();                 //敵の右側。
	CVector3 m_position = CVector3::Zero();             //敵の座標。
	CVector3 m_position_center;                         //当たり判定などで扱う中心座標。
	CQuaternion m_rotation = CQuaternion::Identity();   //キャラの回転
	CQuaternion t_rotation = CQuaternion::Identity();   //キャラの回転
	CQuaternion qBias = CQuaternion::Identity();
	SkinModel m_model;									//スキンモデル。
	SkinModel t_model;//Targetsight。
	EnemyController m_charaCon;                     //敵の剛体。
	Player* player;                                     //プレイヤーのポインタ。
	EnemyState e_state;
	bool enemy_damageflag = false;                          //敵のダメージ判定。
	bool enemy_deathflag = false;                       //敵の死亡フラグ。Managerに使う。
	int enemy_HP;                                       //敵のHP。
	int My_slotNumber;              //EnemyManager上で自身が存在しているスロット番号。
	int count = 0;                     //敵弾の発射間隔。
	int damagecount = 0;
	float enemy_height = 100.0f;		                //敵の高さ。
	float enemy_weight = 30.0f;                         //敵の半径。
	float BulletRange;              //射程範囲。
	float Search_movespeed;         //通常時の移動速度。
};

