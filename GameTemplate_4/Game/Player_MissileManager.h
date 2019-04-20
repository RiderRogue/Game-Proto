#pragma once
#include"Missile.h"
using namespace YTEngine;
class Player_MissileManager
{
public:
	Player_MissileManager();
	~Player_MissileManager();
	void Start();
	void Update();
	//void Draw();
	//void OnDestroy();
	/// <summary>
	///ミサイルのロックオン処理。
	/// </summary>
	/// <remarks>
	/// Playerがミサイルを発射しようとすると呼び出されます。
	/// ミサイルの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射するミサイルの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="p_forward">プレイヤーの前方</param>
	/// <param name="c_forward">カメラの前方</param>
	void missileLock(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward);

	/// <summary>
	///ミサイルの発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがミサイルを発射しようとすると呼び出されます。
	/// ミサイルの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射するミサイルの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="p_forward">プレイヤーの前方</param>
	/// <param name="c_forward">カメラの前方</param>
	void missileShot(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward);

	/*!
	*@brief	敵との当たり判定。
	*/
	bool EnemyHit(CVector3 m_Eposition);

	/*!
	*@brief	インスタンスの取得。
	*/
	static Player_MissileManager& Instance()
	{
		static Player_MissileManager instance;//Player_MissileManagerのインスタンスを生成。
		return instance;
	}

private:
	static const int Player_Missile_NUM = 10;       //ミサイルの配列の数。 
	//Missile missile[Player_Missile_NUM];//ミサイルの配列。
	int missile_ammo_NUM;//ミサイルの弾薬の数。
	static const int Lockon_Enemy_NUM = 3;  //一度にロックオンできる敵の数。
};

//Player_MissileManagerのインスタンスを取得。
static inline Player_MissileManager& G_Player_MissileManager()
{
	return Player_MissileManager::Instance();
}


