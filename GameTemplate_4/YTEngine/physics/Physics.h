#pragma once

namespace YTEngine {
	class RigidBody;

	class CPhysicsWorld
	{
		btDefaultCollisionConfiguration*		collisionConfig = nullptr;
		btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<�Փˉ��������B
		btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
		btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
		btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<���[���h�B
	public:
		~CPhysicsWorld();
		void Init();
		void Update();
		void Release();
		/*!
		* @brief	�_�C�i�~�b�N���[���h���擾�B
		*/
		btDiscreteDynamicsWorld* GetDynamicWorld()
		{
			return dynamicWorld;
		}
		/*!
		* @brief	���̂�o�^�B
		*/
		void AddRigidBody(RigidBody& rb);
		/*!
		* @brief	���̂�j���B
		*/
		void RemoveRigidBody(RigidBody& rb);
		void ConvexSweepTest(
			const btConvexShape* castShape,
			const btTransform& convexFromWorld,
			const btTransform& convexToWorld,
			btCollisionWorld::ConvexResultCallback& resultCallback,
			btScalar allowedCcdPenetration = 0.0f
		)
		{
			dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
		}
		void ContactText(
			btCollisionObject* colObj,
			btCollisionWorld::ContactResultCallback& resultCallback
		)
		{
			dynamicWorld->contactTest(colObj, resultCallback);
		}
	};

	extern CPhysicsWorld g_physics;
}
