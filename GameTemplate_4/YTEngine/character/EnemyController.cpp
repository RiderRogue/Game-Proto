#include "stdafx.h"
#include "EnemyController.h"
#include "Physics/CollisionAttr.h"

namespace YTEngine {

	namespace {
		//�e���Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g
		struct SweepResultEnemyBullet : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//�Փ˃t���O�B
			btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
																//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me 
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_EnemyBullet) {
					//�����ɏՓ˂����B
					return 0.0f;
				}
				//�X�e�[�W�̂ǂ����ƏՓ˂����B
				//�Փ˂��Ă���B
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
		//�R���W�����쐬�B
		m_bullet_radius = radius;
		m_bullet_collider.Create(radius);
		//���̂��������B
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_bullet_collider;
		rbInfo.mass = 0.0f;
		m_bullet_rigidBody.Create(rbInfo);
		btTransform& trans = m_bullet_rigidBody.GetBody()->getWorldTransform();
		//���̂̈ʒu���X�V�B
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
		//�J�v�Z���R���C�_�[�̒��S���W + ����*0.1�̍��W��posTmp�ɋ��߂�B
		CVector3 posTmp = m_bullet_position;
		posTmp.y += m_bullet_radius * 0.5f + m_bullet_radius + m_bullet_radius * 0.1f;
		//���C���쐬�B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
		start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
		//�I�_�̓v���C���[�̍��W�B
		end.setOrigin(btVector3(PlayerVector.x, PlayerVector.y, PlayerVector.z));
		SweepResultEnemyBullet callback;
		callback.me = m_bullet_rigidBody.GetBody();
		//�Փˌ��o�B
		g_physics.ConvexSweepTest((const btConvexShape*)m_bullet_collider.GetBody(), start, end, callback);
		if (callback.isHit) {
			//�X�e�[�W�ɓ��������B
			//�Փ˂����̂�true��Ԃ��B
			return true;
		}
		else {
			//�X�e�[�W�ɓ�����Ȃ������B
			//�Փ˂��Ȃ������̂�true��Ԃ��B
			return false;
		}
	}

	/*!
	* @brief	���S�������Ƃ�ʒm�B
	*/
	void EnemyController::RemoveRigidBoby()
	{
		g_physics.RemoveRigidBody(m_rigidBody);
		g_physics.RemoveRigidBody(m_bullet_rigidBody);
	}

}