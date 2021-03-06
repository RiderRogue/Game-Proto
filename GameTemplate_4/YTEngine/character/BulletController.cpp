#include "stdafx.h"
#include "BulletController.h"
#include "Physics/CollisionAttr.h"
namespace YTEngine {
	namespace {
		//弾が衝突したときに呼ばれる関数オブジェクト
		struct SweepResultBullet : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//衝突フラグ。
			bool E_bulletisHit = false;          //敵弾とプレイヤーの衝突フラグ。
			btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
																//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (me->getUserIndex() == enCollisionAttr_EnemyBullet
					&& convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player) {
					E_bulletisHit = true;
				}
				if (me->getUserIndex() == enCollisionAttr_EnemyBullet
					&& convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_PlayerBullet) {
					isHit = true;
				}
				if (me->getUserIndex() == enCollisionAttr_PlayerBullet
					&& convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_EnemyBullet) {
					isHit = true;
				}
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex()== enCollisionAttr_Player
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_EnemyBullet) {
					//自分に衝突した。
					return 0.0f;
				}
				//ステージのどこかと衝突した。
				//衝突している。
				isHit = true;
			}
		};

		//衝突したときに呼ばれる関数オブジェクト(反射)
		struct SweepReflectBullet : public btCollisionWorld::ConvexResultCallback
		{
			bool E_bulletisHit = false;          //敵弾とプレイヤーの衝突フラグ。
			bool isHit = false;						//衝突フラグ。
			CVector3 hitPos = CVector3::Zero();		//衝突点。
			CVector3 startPos = CVector3::Zero();		//レイの始点。
			float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
			CVector3 hitNormal = CVector3::Zero();	//衝突点の法線。
			btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
													//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (me->getUserIndex() == enCollisionAttr_EnemyBullet
					&& convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player) {
					E_bulletisHit = true;
				}
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_EnemyBullet
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_PlayerBullet
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy) {
					//自分に衝突した。or 地面に衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				CVector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				

				isHit = true;
				CVector3 hitPosTmp;
				hitPosTmp.Set(convexResult.m_hitPointLocal);
				//交点との距離を調べる。
				CVector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				vDist.y = 0.0f;
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = hitNormalTmp;
				}

				return 0.0f;
			}
		};
	}

	BulletController::~BulletController() {
		if (deathflag == false) {
			RemoveRigidBoby();
		}
	}
	void BulletController::Init(float radius, const CVector3& position)
	{
		deathflag = false;
		hitFlag = false;
		m_position = position;
		//コリジョン作成。
		m_radius = radius;
		m_collider.Create(radius);

		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Create(rbInfo);
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		//@todo 未対応。trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		g_physics.AddRigidBody(m_rigidBody);
	}

	const CVector3& BulletController::Execute(float deltaTime, CVector3& moveSpeed)
	{
		//このフレームでの衝突をはかるために初期化。
		E_bullethitFlag = false;
		hitFlag = false;
		//次の移動先となる座標を計算する。
		CVector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。オイラー積分。
		CVector3 addPos = moveSpeed;
		addPos *= deltaTime;
		nextPosition += addPos;
		{
			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				CVector3 addPos;
				addPos.Subtract(nextPosition, m_position);

				//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
				CVector3 posTmp = m_position;
				posTmp.y += m_radius * 0.5f + m_radius + m_radius * 0.1f;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();

				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
				end.setOrigin(btVector3(nextPosition.x, nextPosition.y, nextPosition.z));
				SweepResultBullet callback;
				callback.me = m_rigidBody.GetBody();
				//衝突検出。
				g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				if (callback.E_bulletisHit) {
					//敵弾がプレイヤーに当たった。
					//衝突したのでtrueを返す。
					E_bullethitFlag = true;
				}
				if (callback.isHit) {
					//当たった。
					//壁。
					//衝突したのでtrueを返す。
					hitFlag = true;
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		//移動確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
		//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		return m_position;
	}

	const CVector3& BulletController::ReflectExecute(float deltaTime, CVector3& moveSpeed)
	{
		//このフレームでの衝突をはかるために初期化。
		E_bullethitFlag = false;
		hitFlag = false;
		//次の移動先となる座標を計算する。
		CVector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。オイラー積分。
		CVector3 addPos = moveSpeed;
		addPos *= deltaTime;
		nextPosition += addPos;
		{
			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				CVector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				float Length=addPos.Length();

				//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
				CVector3 posTmp = m_position;
				posTmp.y += m_radius * 0.5f + m_radius + m_radius * 0.1f;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();

				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));
				SweepReflectBullet callback;
				callback.me = m_rigidBody.GetBody();
				//衝突検出。
				g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				if (callback.E_bulletisHit) {
					//敵弾がプレイヤーに当たった。
					//衝突したのでtrueを返す。
					E_bullethitFlag = true;
				}
				if (callback.isHit) {
					//当たった。
					//壁。
					//衝突したのでtrueを返す。
					hitFlag = true;
					CVector3 Reflect;//反射ベクトル。
					Reflect = CVector3::calcReflectVector(moveSpeed, callback.hitNormal);
					moveSpeed = Reflect;
					Reflect *= Length;
					nextPosition = m_position + Reflect;
				}
				else {
					//どことも当たらないので終わり。
					CVector3 moveVector= moveSpeed;
					moveVector.Normalize();
					moveSpeed = moveVector;
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		//移動確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
		//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		return m_position;
	}
	/*!
	* @brief	死亡したことを通知。
	*/
	void BulletController::RemoveRigidBoby()
	{
		//if (m_rigidBody.GetBody() != nullptr) {
		g_physics.RemoveRigidBody(m_rigidBody);
		//}
		deathflag = true;
	}
}