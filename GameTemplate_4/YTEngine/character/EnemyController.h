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
		* @brief	���s�B
		*@param[in]	deltaTime		�o�ߎ��ԁB�P�ʂ͕b�B
		*@param[in, out]	moveSpeed		�ړ����x�B�����ŏd�͉������v�Z����A���̌��ʂ�moveSpeed�ɔ��f����܂��B
		*@return �ړ���̃L�����N�^�[�̍��W�B
		*/
		const CVector3& EnemyExecute(float deltaTime, CVector3& moveSpeed);

		//�G�ƃv���C���[�̊Ԃɏ�Q�����Ȃ������ׂ�B
		bool EnemyofPlayerVector(CVector3 PlayerVector);

		void Init_bullet(float radius,const CVector3& position);

		CVector3 Getbullet_position()
		{
			return m_bullet_position;
		}

		/*!
		* @brief	���̂𕨗��G���W������폜�B�B
		*/
		void RemoveRigidBoby();
	private:
		SphereCollider		m_bullet_collider;					//�R���C�_�[�B
		RigidBody			m_bullet_rigidBody;					//���́B
		CVector3 m_bullet_position;
		float m_bullet_radius;
	};
}
