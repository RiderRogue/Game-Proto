#include "stdafx.h"
#include "EnemyController.h"
#include "Physics/CollisionAttr.h"

namespace YTEngine {

	namespace {
		//弾が衝突したときに呼ばれる関数オブジェクト
		struct SweepResultEnemyBullet : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//衝突フラグ。
			btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
																//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me 
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_EnemyBullet) {
					//自分に衝突した。
					return 0.0f;
				}
				//ステージのどこかと衝突した。
				//衝突している。
				isHit = true;

				return 0.0f;
			}
		};
	}

	EnemyController::~EnemyController()
	{
		RemoveRigidBoby();
	}

	void EnemyController::Init_bullet(float radius, const CVector3& position)
	{
		m_bullet_position = position;
		m_bullet_position.y += (m_height / 2);
		//コリジョン作成。
		m_bullet_radius = radius;
		m_bullet_collider.Create(radius);
		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_bullet_collider;
		rbInfo.mass = 0.0f;
		m_bullet_rigidBody.Create(rbInfo);
		btTransform& trans = m_bullet_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		m_bullet_rigidBody.GetBody()->setUserIndex(enCollisionAttr_EnemyBullet);
		m_bullet_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		g_physics.AddRigidBody(m_bullet_rigidBody);
	}

	const CVector3& EnemyController::EnemyExecute(float deltaTime, CVector3& moveSpeed)
	{
		Execute(deltaTime,moveSpeed);
		m_bullet_position= { m_position.x,m_position.y + (m_height / 2),m_position.z };
		return m_position;
	}

	bool EnemyController::EnemyofPlayerVector(CVector3 PlayerVector)
	{
		//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
		CVector3 posTmp = m_bullet_position;
		posTmp.y += m_bullet_radius * 0.5f + m_bullet_radius + m_bullet_radius * 0.1f;
		//レイを作成。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
		start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
		//終点はプレイヤーの座標。
		end.setOrigin(btVector3(PlayerVector.x, PlayerVector.y, PlayerVector.z));
		SweepResultEnemyBullet callback;
		callback.me = m_bullet_rigidBody.GetBody();
		//衝突検出。
		g_physics.ConvexSweepTest((const btConvexShape*)m_bullet_collider.GetBody(), start, end, callback);
		if (callback.isHit) {
			//ステージに当たった。
			//衝突したのでtrueを返す。
			return true;
		}
		else {
			//ステージに当たらなかった。
			//衝突しなかったのでtrueを返す。
			return false;
		}
	}

	/*!
	* @brief	死亡したことを通知。
	*/
	void EnemyController::RemoveRigidBoby()
	{
		g_physics.RemoveRigidBody(m_rigidBody);
		g_physics.RemoveRigidBody(m_bullet_rigidBody);
	}

}