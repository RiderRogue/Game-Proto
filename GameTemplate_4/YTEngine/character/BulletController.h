#pragma once
/*!
* @brief	�o���b�g�R���g���[���[�B
*/

#include "Physics/SphereCollider.h"
#include "Physics/RigidBody.h"

namespace YTEngine {
	/*!
	* @brief	�o���b�g�R���g���[���[�B
	*/
	class BulletController
	{
	public:
		BulletController() {

		}
		virtual ~BulletController();

		/*!
		* @brief	�������B
		*@param[in]	radius		�X�t�B�A�R���C�_�[�̔��a�B
		*@param[in]	position	�����ʒu�B
		*/
		virtual void Init(float radius, const CVector3& position);

		/*!
		* @brief	���s�B
		*@param[in]	deltaTime		�o�ߎ��ԁB�P�ʂ͕b�B
		*@param[in, out]	moveSpeed		�ړ����x�B�����ŏd�͉������v�Z����A���̌��ʂ�moveSpeed�ɔ��f����܂��B
		*@return �ړ���̃L�����N�^�[�̍��W�B
		*/
		virtual const CVector3& Execute(float deltaTime, CVector3& moveSpeed);
		/*!
		* @brief	���s�B���ˁB
		*@param[in]	deltaTime		�o�ߎ��ԁB�P�ʂ͕b�B
		*@param[in, out]	moveSpeed		�ړ����x�B�����ŏd�͉������v�Z����A���̌��ʂ�moveSpeed�ɔ��f����܂��B
		*@return �ړ���̃L�����N�^�[�̍��W�B
		*/
		const CVector3& ReflectExecute(float deltaTime, CVector3& moveSpeed);
		/*!
		* @brief	���W���擾�B
		*/
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/*!
		* @brief	���W��ݒ�B
		*/
		void SetPosition(const CVector3& pos)
		{
			m_position = pos;
		}
		/*!
		* @brief	�R���C�_�[���擾�B
		*/
		SphereCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	���̂��擾�B
		*/
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/*!
		* @brief	���̂𕨗��G���W������폜�B�B
		*/
		void RemoveRigidBoby();

		/*!
		* @brief   ���̃t���[���ŏՓ˂������ǂ�����Ԃ��B
		*          ���̂��߁AExecute������ɋL�q���邱�ƁBtrue�ŏՓˁB
		*/
		bool Gethit()
		{
			return hitFlag;
		}

		/*!
		* @brief   ���̃t���[���Ńv���C���[�ɏՓ˂������ǂ�����Ԃ��B
		*          ���̂��߁AExecute������ɋL�q���邱�ƁBtrue�ŏՓˁB
		*/
		bool GetE_bullethit()
		{
			return E_bullethitFlag;
		}
	protected:
		CVector3 			m_position = CVector3::Zero();	//���W�B
		SphereCollider		m_collider;						//�R���C�_�[�B
		float				m_radius = 0.0f;
		RigidBody			m_rigidBody;					//���́B
		int Flag = 0;										//�L�����N�^�[�̃t���O
		bool hitFlag = false;                  //�n�ʂ�ǂɂԂ��������Ƃ�m�点��t���O�B
		bool E_bullethitFlag = false;   //�G�e���v���C���[�ɂԂ��������Ƃ�m�点��t���O�B
		bool deathflag = true;   //�e�����݂��Ă��邩�𔻒肷��B
	};
}
