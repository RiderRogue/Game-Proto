#pragma once
/*!
* @brief	スフィアコライダー。
*/

#include "Physics/ICollider.h"
namespace YTEngine {
	class SphereCollider : public ICollider
	{
	public:
		/*!
		* @brief	コンストラクタ。
		*/
		SphereCollider();
		/*!
		* @brief	デストラクタ。
		*/
		~SphereCollider();
		/*!
		* @brief	作成。
		*@param[in] radius	球体の半径。
		*/
		void Create(float radius)
		{
			shape = new btSphereShape(radius);//, height);
			this->radius = radius;
		}
		btCollisionShape* GetBody() const override
		{
			return shape;
		}
		float GetRadius() const
		{
			return radius;
		}
	private:
		btSphereShape * shape = nullptr;
		float radius = 0.0f; //半径。
	};
}
