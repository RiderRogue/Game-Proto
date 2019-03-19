#pragma once
/*!
* @brief	エネミー(敵)。
*/
#include "Player.h"
#include "character/CharacterController.h"
using namespace YTEngine;
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Draw();
	//シャドウキャスターへの登録。
	void SetShadowCasters();
	/*!
	* @brief	ダメージ＆死亡フラグ。
	*@param[in]	damage		受けたダメージ量。
	*/
	void enemyDamage(int damage);
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
	void SetPosition(CVector3 position)
	{
		m_position = position;
		m_charaCon.SetPosition(position);
		m_position_center = position;
		m_position_center.y += (enemy_height / 2);
	}
	/*!
	*@brief	当たり判定などで扱う、中心座標の取得。
	*/
	CVector3 GetPosition_center()
	{
		return m_position_center;
	}
	/*!
	*@brief	EnemyManagerでの配列番号の保存。
	*/
	void SetslotNumber(int slotNumber)
	{
		My_slotNumber = slotNumber;
	}
private:
	CMatrix mRot;                                       //敵の回転行列。
	CVector3 m_moveSpeed = CVector3::Zero();            //敵の移動速度。
	CVector3 m_forward = CVector3::Zero();				//敵の前方。
	CVector3 m_rite = CVector3::Zero();                 //敵の右側。
	CQuaternion m_rotation = CQuaternion::Identity();   //キャラの回転
	CQuaternion qBias = CQuaternion::Identity();
	CVector3 m_position = CVector3::Zero();             //プレイヤーの座標。
	CVector3 m_position_center;
	SkinModel m_model;									//スキンモデル。
	CharacterController m_charaCon;                     //敵の剛体。
	int enemy_HP;                                       //敵のHP。
	const float enemy_height = 100.0f;		                //敵の高さ。
	const float enemy_weight = 30.0f;                         //敵の半径。
	bool enemy_deathflag = false;                       //敵の死亡フラグ。Managerに使う。
	Player* player;                                     //プレイヤーのポインタ。
	int My_slotNumber;              //EnemyManager上で自身が存在しているスロット番号。
};

