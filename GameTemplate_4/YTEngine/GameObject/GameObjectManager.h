#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Sprite.h"
#include "PostEffect.h"


namespace YTEngine {

	/*!
	*@brief	CGameObject�̃}�l�[�W��
	*/
	class CGameObjectManager : Noncopyable {
	private:
		CGameObjectManager(); 
		
			
		

		~CGameObjectManager()
		{
			if (m_frameBufferRenderTargetView != nullptr) {
				m_frameBufferRenderTargetView->Release();
			}
			if (m_frameBufferDepthStencilView != nullptr) {
				m_frameBufferDepthStencilView->Release();
			}
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̖��O�L�[���쐬�B
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = CUtil::MakeHash("Undefined");	//���O�L�[�B
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = CUtil::MakeHash(objectName);
			}
			return hash;
		}
	public:
		/*!
		*@brief	�C���X�^���X�̎擾�B
		*/
		static CGameObjectManager& Instance()
		{
			static CGameObjectManager instance;//CGameObjectManager�̃C���X�^���X�𐶐��B
			return instance;
		}
		/*!
		*@brief	���s�B
		*/
		void Execute();
		/*!
		*@brief	�������B
		*@param[in]	gameObjectPrioMax	�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B(255�܂�)
		*/
		void Init(int gameObjectPrioMax);
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̒ǉ��B
		*@param[in]	prio			���s�D�揇�ʁB
		*@param[in] go				�Q�[���I�u�W�F�N�g
		*@param[in] objectName		�I�u�W�F�N�g��
		*/
		void AddGameObject(GameObjectPrio prio, IGameObject* go, const char* objectName = nullptr)
		{
			(void)objectName;
			if (!go->m_isRegist) {
				go->Awake();
				unsigned int hash = MakeGameObjectNameKey(objectName);
				m_gameObjectListArray.at(prio).push_back(go);
				go->m_isRegist = true;
				go->m_priority = prio;
				go->m_isStart = false;
				go->m_nameKey = hash;
				if (go->m_isDead) {
					//���S�t���O�������Ă���B
					//�폜���X�g�ɓ����Ă����炻�����珜������B
					go->m_isDead = false;
				}

			}
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g��new
		*@details
		* ���̊֐����g�p����new�����I�u�W�F�N�g�͕K��DeleteGameObject�����s���邱�Ƃ�delete����܂��B
		*@param[in]	prio		���s�D�揇�ʁB
		*@param[in]	objectName	�I�u�W�F�N�g���B
		*@param[in]	ctorArgs	�R���X�g���N�^�ɓn���ϒ������B
		*/
		template<class T, class... TArgs>
		T* NewGameObject(GameObjectPrio prio, const char* objectName, TArgs... ctorArgs)
		{
			(void*)objectName;
			//TK_ASSERT(prio <= m_gameObjectPriorityMax, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
			T* newObject = new T(ctorArgs...);
			newObject->Awake();
			newObject->SetMarkNewFromGameObjectManager();
			m_gameObjectListArray.at(prio).push_back(newObject);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObject->m_isRegist = true;
			newObject->m_priority = prio;
			newObject->m_nameKey = hash;
			return newObject;
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̍폜�B
		*/
		void DeleteGameObject(IGameObject* gameObject)
		{
			if (gameObject != nullptr
				&& gameObject->m_isRegist
				) {
				gameObject->SetDeadMark();
				gameObject->OnDestroy();
				gameObject->m_isRegist = false;
				gameObject->m_isRegistDeadList = true;

				m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
				gameObject = nullptr;
			}
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̌����B
		*@details
		* �d����I
		*@param[in]	objectName		�I�u�W�F�N�g���B
		*/
		template<class T>
		T* FindGameObject(const char* objectName)
		{
			unsigned int nameKey = CUtil::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//�������B
						return dynamic_cast<T*>(go);
					}
				}
			}
			//������Ȃ������B
			return nullptr;
		}
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̌����̃w���p�[�֐��B
		*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
		*/
		template<class T>
		static inline T* FindGO(const char* objectName)
		{
			return GameObjectManager().FindGameObject<T>(objectName);
		}
		/*!
		*@brief	�w�肵���^�O�̂����ꂩ�����܂܂��Q�[���I�u�W�F�N�g���������āA���������ꍇ�w�肳�ꂽ�R�[���o�b�N�֐����Ăяo���B
		*/

		void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)>func)
		{
			for (auto& goList : m_gameObjectListArray) {
				for (auto& go : goList) {
					unsigned int goTags = go->GetTags();
					if ((goTags & tags) != 0) {
						func(go);
					}
				}
			}

		}

		/*!
		*@brief �Q�[���I�u�W�F�N�g���V���h�E�L���X�^�[�Ɋi�[����B
		*/
		void SetObjectShadowCasters();

		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g���擾�B
		/// </summary>
		/// <returns></returns>
		RenderTarget* GetMainRenderTarget()
		{
			return &m_mainRenderTarget;
		}
	private:
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̍폜�����s�B
		*/
		void ExecuteDeleteGameObjects();
		/*!
		*@brief	�V�[���O���t�̍X�V�B
		*/
		void UpdateSceneGraph();
	private:
		RenderTarget m_mainRenderTarget;		//���C�������_�����O�^�[�Q�b�g�B
		PostEffect m_postEffect;				//�|�X�g�G�t�F�N�g�B
		Sprite m_copyMainRtToFrameBufferSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�B
		D3D11_VIEWPORT m_frameBufferViewports;         //�t���[���o�b�t�@�̃r���[�|�[�g�B
		CTransform m_transform;												//!<Transform�B
		typedef std::list<IGameObject*>	GameObjectList;
		std::vector<GameObjectList>	m_gameObjectListArray;					//!<�Q�[���I�u�W�F�N�g�̗D��x�t�����X�g�B
		std::vector<GameObjectList>	m_deleteObjectArray[2];					//!<�폜����I�u�W�F�N�g�̃��X�g�B�폜�������s���Ă���Œ���DeleteGameObject���Ă΂��\���������̂ŁA�_�u���o�b�t�@���B
		std::list<CTransform*>		m_childrenOfRootTransformList;			//!<���[�g�̎q����Transform�̃��X�g�B
		GameObjectPrio				m_gameObjectPriorityMax;				//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�B
		int m_currentDeleteObjectBufferNo = 0;								//!<���݂̍폜�I�u�W�F�N�g�̃o�b�t�@�ԍ��B
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B
		ID3D11RenderTargetView* m_frameBufferRenderTargetView;
		ID3D11DepthStencilView* m_frameBufferDepthStencilView;
	};

	//CGameObjectManager�̃C���X�^���X���擾�B
	static inline CGameObjectManager& GameObjectManager()
	{
		return CGameObjectManager::Instance();
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
	*@param[in]	priority	�v���C�I���e�B�B
	*@param[in]	objectName	�I�u�W�F�N�g���B(NULL�̎w��j
	*@param[in]	ctorArgs	�R���X�g���N�^�ɓn���ϒ������B
	*/
	template<class T, class... TArgs>
	static inline T* NewGO(int priority, const char* objectName, TArgs... ctorArgs)
	{
		return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName, ctorArgs...);
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
	*@details
	* �폜���B
	*@param[in]	priority	�v���C�I���e�B�B
	*@param[in]	objectName	�I�u�W�F�N�g���B(NULL�̎w��j
	*@param[in]	ctorArgs	�R���X�g���N�^�ɓn���ϒ������B
	*/
	template<class T>
	static inline T* NewGO(int priority)
	{
		return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, nullptr);
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐��B
	* NewGO���g�p���č쐬�����I�u�W�F�N�g�͕K��DeleteGO�����s����悤�ɁB
	*@param[in]	go		�폜����Q�[���I�u�W�F�N�g�B
	*/
	static inline void DeleteGO(IGameObject* go)
	{
		GameObjectManager().DeleteGameObject(go);
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�̒ǉ��̃w���p�[�֐��B
	*@param[in]	go			�ǉ�����Q�[���I�u�W�F�N�g�B
	*@param[in]	priority	�v���C�I���e�B�B
	*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
	*/
	static inline void AddGO(int priority, IGameObject* go, const char* objectName = nullptr)
	{
		GameObjectManager().AddGameObject(static_cast<GameObjectPrio>(priority), go, objectName);
	}
	/*!
	*@brief	�w�肵���^�O�̂����ꂩ�����܂܂��Q�[���I�u�W�F�N�g���������āA���������ꍇ�w�肳�ꂽ�R�[���o�b�N�֐����Ăяo���B
	*/
	static inline 	void FindGameObjectsWithTag(unsigned int tags, std::function<void(IGameObject* go)>func)
	{
		GameObjectManager().FindGameObjectsWithTag(tags, func);
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�̌����̃w���p�[�֐��B
	*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
	*/
	template<class T>
	static inline T* FindGO(const char* objectName)
	{
		return GameObjectManager().FindGameObject<T>(objectName);
	}
}
