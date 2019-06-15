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

		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
		struct SweepReflectWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//�Փ˃t���O�B
			CVector3 hitPos = CVector3::Zero();		//�Փ˓_�B
			CVector3 startPos = CVector3::Zero();		//���C�̎n�_�B
			float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
			CVector3 hitNormal = CVector3::Zero();	//�Փ˓_�̖@���B
			btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
													//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (/*convexResult.m_hitCollisionObject == me
					||*/ convexResult.m_hitCollisionObject->getUserIndex() != -1
					//|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Player
					/*|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Enemy*/) {
					//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
					return 0.0f;
				}
				//�Փ˓_�̖@�������������Ă���B
				CVector3 hitNormalTmp;
				hitNormalTmp.Set(convexResult.m_hitNormalLocal);
				//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
				float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up())));
				if (angle >= CMath::PI * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
					) {
					isHit = true;
					CVector3 hitPosTmp;
					hitPosTmp.Set(convexResult.m_hitPointLocal);
					//��_�Ƃ̋����𒲂ׂ�B
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					vDist.y = 0.0f;
					float distTmp = vDist.Length();
					if (distTmp < dist) {
						//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
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
	//�ǔ��ˁB
	CVector3 EnemyController::ReflectWall(CVector3 moveSpeed)
	{
		CVector3 nextPosition = m_position;
		//���x����10�t���[����ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
		CVector3 addPos = moveSpeed;
		float speed = addPos.Length();
		addPos *= 3.0f;//10�t���[���B
		nextPosition += addPos;
		
		CVector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
		addPos.Subtract(nextPosition, m_position);
		CVector3 addPosXZ = addPos;
		addPosXZ.y = 0.0f;
		if (addPosXZ.Length() < FLT_EPSILON) {
			//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
			//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
			//�ƂĂ��������l�̂��Ƃł��B
			CVector3 returnVector = moveSpeed;
			returnVector.Normalize();
			returnVector *= speed;
			return returnVector;//moveSpeed�𐳋K���������̂�Ԃ��B
		}
		//�J�v�Z���R���C�_�[�̒��S���W + ����*0.1�̍��W��posTmp�ɋ��߂�B
		CVector3 posTmp = m_position;
		posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
		//���C���쐬�B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
		start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
		//�I�_�͎��̈ړ���B
		end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

		SweepReflectWall callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos = posTmp;
		//�Փˌ��o�B
		g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
		if (callback.isHit) {
			//���������B
			//�ǁB
			//�Փ˂����ǂ̖@�������߂�B�B
			CVector3 hitNormalXZ = callback.hitNormal;
			hitNormalXZ.y = 0.0f;
			hitNormalXZ.Normalize();
			addPos.Normalize();
			//�ړ������̒P�ʃx�N�g���ƕǂ̖@���̓��ς����߂�B
			float degree = CMath::RadToDeg(acosf(addPos.Dot(hitNormalXZ)));
			if (degree >= 30.0f) {
				//�ǂ̖@��
				CVector3 nextVector = addPos + hitNormalXZ;
				nextVector.Normalize();
				nextVector *= speed;

				nextPosition = m_position + nextVector;
				CVector3 currentDir;
				currentDir = nextPosition - m_position;
				currentDir.y = 0.0f;
				currentDir.Normalize();
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//�p�ɓ��������̃L�����N�^�̐U����h�~���邽�߂ɁA
					//�ړ��悪�t�����ɂȂ�����ړ����L�����Z������B
					nextVector = CVector3::Zero();
					
				}

				return nextVector;
			}
			else {
				//�ǂƈړ������̒P�ʃx�N�g�����^�t�̂Ƃ�
				//��]�B
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
			return returnVector;//moveSpeed�𐳋K���������̂�Ԃ��B
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