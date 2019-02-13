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
	*@brief	死亡フラグの取得。
	*/
	bool Getenemy_deathflag()
	{
		return enemy_deathflag;
	}
private:
	CMatrix mRot;                                       //敵の回転行列。
	CVector3 m_moveSpeed = CVector3::Zero();            //敵の移動速度。
	CVector3 m_forward = CVector3::Zero();				//敵の前方。
	CVector3 m_rite = CVector3::Zero();                 //敵の右側。
	CQuaternion m_rotation = CQuaternion::Identity();   //キャラの回転
	CQuaternion qBias = CQuaternion::Identity();
	CVector3 m_position = CVector3::Zero();             //プレイヤーの座標。
	SkinModel m_model;									//スキンモデル。
	CharacterController m_charaCon;                     //敵の剛体。
	float enemy_height = 100.0f;		                //敵の高さ。
	float enemy_weight = 30.0f;                         //敵の半径。
	bool enemy_deathflag = false;                       //敵の死亡フラグ。Managerに使う。
	Player* player;                                     //プレイヤーのポインタ。
};

