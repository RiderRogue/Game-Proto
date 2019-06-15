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

		//衝突したときに呼ばれる関数オブジェクト(壁用)
		struct SweepReflectWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//衝突フラグ。
			CVector3 hitPos = CVector3::Zero();		//衝突点。
			CVector3 startPos = CVector3::Zero();		//レイの始点。
			float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
			CVector3 hitNormal = CVector3::Zero();	//衝突点の法線。
			btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
													//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (/*convexResult.m_hitCollisionObject == me
					||*/ convexResult.m_hitCollisionObject->getUserIndex() != -1
					//|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player
					/*|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy*/) {
					//自分に衝突した。or 地面に衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				CVector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//上方向と衝突点の法線のなす角度を求める。
				float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up())));
				if (angle >= CMath::PI * 0.3f		//地面の傾斜が54度以上なので壁とみなす。
					) {
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
				}
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
	//壁反射。
	CVector3 EnemyController::ReflectWall(CVector3 moveSpeed)
	{
		CVector3 nextPosition = m_position;
		//速度から10フレーム先での移動量を求める。オイラー積分。
		CVector3 addPos = moveSpeed;
		float speed = addPos.Length();
		addPos *= 3.0f;//10フレーム。
		nextPosition += addPos;
		
		CVector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//現在の座標から次の移動先へ向かうベクトルを求める。
		addPos.Subtract(nextPosition, m_position);
		CVector3 addPosXZ = addPos;
		addPosXZ.y = 0.0f;
		if (addPosXZ.Length() < FLT_EPSILON) {
			//XZ平面で動きがないので調べる必要なし。
			//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
			//とても小さい値のことです。
			CVector3 returnVector = moveSpeed;
			returnVector.Normalize();
			returnVector *= speed;
			return returnVector;//moveSpeedを正規化したものを返す。
		}
		//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
		CVector3 posTmp = m_position;
		posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
		//レイを作成。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
		start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
		//終点は次の移動先。
		end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

		SweepReflectWall callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos = posTmp;
		//衝突検出。
		g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
		if (callback.isHit) {
			//当たった。
			//壁。
			//衝突した壁の法線を求める。。
			CVector3 hitNormalXZ = callback.hitNormal;
			hitNormalXZ.y = 0.0f;
			hitNormalXZ.Normalize();
			addPos.Normalize();
			//移動方向の単位ベクトルと壁の法線の内積を求める。
			float degree = CMath::RadToDeg(acosf(addPos.Dot(hitNormalXZ)));
			if (degree >= 30.0f) {
				//壁の法線
				CVector3 nextVector = addPos + hitNormalXZ;
				nextVector.Normalize();
				nextVector *= speed;

				nextPosition = m_position + nextVector;
				CVector3 currentDir;
				currentDir = nextPosition - m_position;
				currentDir.y = 0.0f;
				currentDir.Normalize();
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//角に入った時のキャラクタの振動を防止するために、
					//移動先が逆向きになったら移動をキャンセルする。
					nextVector = CVector3::Zero();
					
				}

				return nextVector;
			}
			else {
				//壁と移動方向の単位ベクトルが真逆のとき
				//回転。
				CQuaternion qBias;
				qBias.SetRotationDeg(CVector3::AxisY(), 90.0f);
				qBias.Multiply(addPos);
				addPos *= speed;
				return addPos;
			}
		}
		else {
			CVector3 returnVector = moveSpeed;
			returnVector.Normalize();
			returnVector *= speed;
			return returnVector;//moveSpeedを正規化したものを返す。
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