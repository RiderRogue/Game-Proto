#pragma once

#include "character/BulletController.h"
#include "Enemy.h"
using namespace YTEngine;
struct Player_BulletState {
	bool flag;//弾丸が発射中かどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
};

struct Player_MineState {
	bool flag;//マインが設置中かどうか
	bool explosion; //爆発したかどうか
	CVector3 m_position;
	SkinModel m_model;				  //スキンモデル。
	float blastrange;                 //爆発範囲。
	bool desflag;                     //生存しているか？
};

struct Player_Blackhole {
	bool flag;//弾丸が発射中かどうか
	bool explosion; //爆発したかどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
	float time;
};
struct Player_MissileState {
	bool flag;//弾丸が発射中かどうか
	CVector3 m_position;
	CVector3 m_forward;
	SkinModel m_model;				   //スキンモデル。
	BulletController m_bulletCon;    //弾丸の剛体。
	int lengthcount;                   //弾丸の移動距離。
	bool desflag;                      //生存しているか？
	int lockon_enemyNumber;          //ロックオンした敵のスロット番号。
};
class Player_BulletManager
{
public:
	Player_BulletManager();
	~Player_BulletManager();
	void Start();
	void Update();
	void Draw();
	void OnDestroy();
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
	void bulletShot(const CVector3 p_position, const CVector3 p_forward);

	/// <summary>
	///マインの発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがマインを設置しようとすると呼び出されます。
	/// マインの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから設置するマインの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	void mineShot(const CVector3 p_position);

	void mineexplosion();

	/// <summary>
	///ブラックホール弾の発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがブラックホール弾を発射しようとすると呼び出されます。
	/// 弾丸の配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射する弾丸の情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void BlackholeShot(const CVector3 p_position, const CVector3 p_forward);

	/// <summary>
	///ミサイルの発射処理。
	/// </summary>
	/// <remarks>
	/// Playerがミサイルを発射しようとすると呼び出されます。
	/// ミサイルの配列からflagがfalseの要素を見つけ、
	/// その要素にこれから発射するミサイルの情報を格納してflagをtrueにします。
	/// </remarks>
	/// <param name="p_position">プレイヤーの座標</param>
	/// <param name="lightDir">プレイヤーの前方</param>
	void missileShot(const CVector3 p_position, const CVector3 p_forward);
	/*!
	*@brief	通常弾の制御。
	*/
	void Normalbullet_move(int bulletNumber);
	/*!
	*@brief	ミサイルの制御。
	*/
	void missile_move(int missiletNumber);

	/*!
	*@brief	ブラックホール弾の制御。
	*/
	void Blackhole_move(int bulletNumber);
	/*!
	*@brief	敵との当たり判定。
	*/
	void EnemyHit(Enemy* enemy);

	/*!
	*@brief	敵との当たり判定(マイン)。
	*/
	void EnemyHitMine(Enemy* enemy);

	/*!
	*@brief	敵との当たり判定(ブラックホール)。
	*/
	void EnemyHitBlackhole(Enemy* enemy);

	/*!
	*@brief	インスタンスの取得。
	*/
	static Player_BulletManager& Instance()
	{
		static Player_BulletManager instance;//Player_BulletManagerのインスタンスを生成。
		return instance;
	}
private:
	static const int Player_Bullet_NUM = 10;       //弾丸の配列の数。
	static const int Player_Mine_NUM = 5;          //マインの配列の数。
	static const int Player_Blackhole_NUM = 1;     //ブラックホールの配列の数。
	static const int Player_Missile_NUM = 10;      //ミサイルの配列の数。
	const float bulletmoveSpeed = 10000.0f;           //弾速。
	const float blackholeAbsorb = 2000.0f;         //ブラックホールの吸収範囲。
	const float blackholeTime = 300.0f;         //ブラックホールの生成時間。
	const int BulletDamage = 10;
	const int MineDamage = 100;
	const int BlackholeDamage = 5;
	int count = 0;
	int minecount = 0;
	int blackholecount = 0;
	int missile_ammo_NUM;//ミサイルの弾薬の数。
	Player_BulletState bullet[Player_Bullet_NUM];  //弾丸の配列。    
	Player_MineState mine[Player_Mine_NUM];  //マインの配列。
	Player_Blackhole blackhole[Player_Blackhole_NUM];  //ブラックホールの配列。
	Player_MissileState missile[Player_Missile_NUM];//ミサイルの配列。
	SkinModel m_modelproto;						   //スキンモデルのプロトタイプ。

	Effekseer::Effect* m_sampleEffect = nullptr;
	Effekseer::Effect* m_blackholeEffect = nullptr;
	Effekseer::Handle m_mineEffectHandle = -1;
	//Effekseer::Handle m_pBulletEffectHandle = -1;
};

//Player_BulletManagerのインスタンスを取得。
static inline Player_BulletManager& G_Player_BulletManager()
{
	return Player_BulletManager::Instance();
}
