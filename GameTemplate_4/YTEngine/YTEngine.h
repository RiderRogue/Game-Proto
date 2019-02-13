#pragma once
#include "ShadowMap.h"
namespace YTEngine {
	/*!
	*@brief	�G���W���B
	*/
	class CEngine : Noncopyable {
	private:
		/*!
		*@brief	�R���X�g���N�^�B
		*/
		CEngine();
		/*!
		*@brief	�f�X�g���N�^�B
		*/
		~CEngine();
	public:
		/*!
		*@brief	�������B
		*/
		bool Init(int gameObjectPrioMax);
		/*!
		*@brief	�Q�[�����[�v�����s�B
		*/
		void RunGameLoop();
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/*!
		*@brief	�I�������B
		*/
		void Final();
		/*!
		*@brief	�C���X�^���X���擾�B
		*/
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;//�G���W���̃C���X�^���X���쐬�B
			}
			return *instance;
		}

		/// <summary>
		/// �V���h�E�}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		ShadowMap* GetShadowMap()
		{
			return &m_shadowMap;
		}
		
	private:
		
		ShadowMap m_shadowMap;					//�V���h�E�}�b�v�B
		
	};

	/*!
	* @brief	Engine�̃C���X�^���X���擾�B
	*/
	static inline CEngine& Engine()
	{
		return CEngine::GetInstance();
	}

}