#pragma once
#include "Physics/SphereCollider.h"
#include"character/CharacterController.h"
namespace YTEngine {
	class EnemyController : public CharacterController
	{
	public:
		EnemyController() {}
		~EnemyController();

		/*!
		* @brief	実行。
		*@param[in]	deltaTime		経過時間。単位は秒。
		*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
		*@return 移動後のキャラクターの座標。
		*/
		const CVector3& EnemyExecute(float deltaTime, CVector3& moveSpeed);

		//敵とプレイヤーの間に障害物がないか調べる。
		bool EnemyofPlayerVector(CVector3 PlayerVector);

		void Init_bullet(float radius,const CVector3& position);

		CVector3 Getbullet_position()
		{
			return m_bullet_position;
		}

		/*!
		* @brief	剛体を物理エンジンから削除。。
		*/
		void RemoveRigidBoby();
	private:
		SphereCollider		m_bullet_collider;					//コライダー。
		RigidBody			m_bullet_rigidBody;					//剛体。
		CVector3 m_bullet_position;
		float m_bullet_radius;
	};
}
