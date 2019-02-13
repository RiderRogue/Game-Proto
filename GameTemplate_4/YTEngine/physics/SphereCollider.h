#pragma once
/*!
* @brief	�X�t�B�A�R���C�_�[�B
*/

#include "Physics/ICollider.h"
namespace YTEngine {
	class SphereCollider : public ICollider
	{
	public:
		/*!
		* @brief	�R���X�g���N�^�B
		*/
		SphereCollider();
		/*!
		* @brief	�f�X�g���N�^�B
		*/
		~SphereCollider();
		/*!
		* @brief	�쐬�B
		*@param[in] radius	���̂̔��a�B
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
		float radius = 0.0f; //���a�B
	};
}
