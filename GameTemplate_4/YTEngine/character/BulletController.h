#pragma once
/*!
* @brief	バレットコントローラー。
*/

#include "Physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace YTEngine {
	/*!
	* @brief	バレットコントローラー。
	*/
	class BulletController
	{
	public:
		BulletController() {

		}
		virtual ~BulletController();

		/*!
		* @brief	初期化。
		*@param[in]	radius		スフィアコライダーの半径。
		*@param[in]	position	初期位置。
		*/
		virtual void Init(float radius, const CVector3& position);

		/*!
		* @brief	実行。
		*@param[in]	deltaTime		経過時間。単位は秒。
		*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
		*@return 移動後のキャラクターの座標。
		*/
		virtual const CVector3& Execute(float deltaTime, CVector3& moveSpeed);
		/*!
		* @brief	実行。反射。
		*@param[in]	deltaTime		経過時間。単位は秒。
		*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
		*@return 移動後のキャラクターの座標。
		*/
		const CVector3& ReflectExecute(float deltaTime, CVector3& moveSpeed);
		/*!
		* @brief	座標を取得。
		*/
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		* @brief	座標を設定。
		*/
		void SetPosition(const CVector3& pos)
		{
			m_position = pos;
		}
		/*!
		* @brief	コライダーを取得。
		*/
		SphereCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	剛体を取得。
		*/
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/*!
		* @brief	剛体を物理エンジンから削除。。
		*/
		void RemoveRigidBoby();

		/*!
		* @brief   このフレームで衝突したかどうかを返す。
		*          そのため、Executeよりも後に記述すること。trueで衝突。
		*/
		bool Gethit()
		{
			return hitFlag;
		}

		/*!
		* @brief   このフレームでプレイヤーに衝突したかどうかを返す。
		*          そのため、Executeよりも後に記述すること。trueで衝突。
		*/
		bool GetE_bullethit()
		{
			return E_bullethitFlag;
		}
	protected:
		CVector3 			m_position = CVector3::Zero();	//座標。
		SphereCollider		m_collider;						//コライダー。
		float				m_radius = 0.0f;
		RigidBody			m_rigidBody;					//剛体。
		int Flag = 0;										//キャラクターのフラグ
		bool hitFlag = false;                  //地面や壁にぶつかったことを知らせるフラグ。
		bool E_bullethitFlag = false;   //敵弾がプレイヤーにぶつかったことを知らせるフラグ。
		bool deathflag = true;   //弾が存在しているかを判定する。
	};
}
