#pragma once

#include "character/BulletController.h"
struct Player_BulletState {
	bool flag;//弾丸が発射中かどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_charaCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
};

class Player_BulletManager : public IGameObject
{
public:
	Player_BulletManager();
	~Player_BulletManager();
	bool Start();
	void Update();
	void Draw();
	/// <summary>
	///弾丸の発射処理。
	/// </summary>
	/// <remarks>
	/// Playerが弾丸を発射しようとすると呼び出されます。
	/// 弾丸の配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void Shot(const CVector3 p_position, const CVector3 p_forward);
private:
	static const int Player_Bullet_NUM = 10;       //弾丸の配列の数。 
	Player_BulletState bullet[Player_Bullet_NUM];  //弾丸の配列。    
	SkinModel m_modelproto;						   //スキンモデルのプロトタイプ。
	const float bulletmoveSpeed = 40.0f;           //弾速。
	int count = 0;
};


