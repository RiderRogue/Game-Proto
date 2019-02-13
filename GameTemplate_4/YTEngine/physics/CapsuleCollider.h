/*!
* @brief	�J�v�Z���R���C�_�[�B
*/

#pragma once

#include "Physics/ICollider.h"

namespace YTEngine {
	class CapsuleCollider : public ICollider
	{
	public:
		/*!
		* @brief	�R���X�g���N�^�B
		*/
		CapsuleCollider();
		/*!
		* @brief	�f�X�g���N�^�B
		*/
		~CapsuleCollider();
		/*!
		* @brief	�쐬�B
		*@param[in] radius	�J�v�Z���̔��a�B
		*@param[in]	height	�J�v�Z���̍����B
		*/
		void Create(float radius, float height)
		{
			shape = new btCapsuleShape(radius, height);//, height);
			this->radius = radius;
			this->height = height;
		}
		btCollisionShape* GetBody() const override
		{
			return shape;
		}
		float GetRadius() const
		{
			return radius;
		}
		float GetHeight() const
		{
			return height;
		}
	private:
		btCapsuleShape * shape = nullptr;
		float radius = 0.0f; //���a
		float height = 0.0f; //����
	};
}