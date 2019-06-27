#pragma once
namespace YTEngine {
	class EffekseerManager
	{
	public:
		EffekseerManager();
		~EffekseerManager();
		/*!
		*@brief	�������B
		*/
		void Init();

		void Update();
		void Draw();
		void OnDestroy();

		//�G�t�F�N�g�̍Đ��B
		Effekseer::Handle Play(Effekseer::Effect* effect, CVector3 EffectPos)
		{
			return m_effekseerManager->Play(effect, EffectPos.x, EffectPos.y, EffectPos.z);
		}
		/*Effekseer::Handle Play(Effekseer::Effect* effect, float x, float y, float z)
		{
			return m_effekseerManager->Play(effect, x, y, z);
		}*/
		//�Đ����̃G�t�F�N�g���~�߂�B
		void StopEffect(Effekseer::Handle handle)
		{
			m_effekseerManager->StopEffect(handle);
		}

		//�Đ����̃G�t�F�N�g�̍��W��ݒ�B
		void SetEffectposition(Effekseer::Handle handle,CVector3 pos)
		{
			m_effekseerManager->SetLocation(handle, pos.x, pos.y, pos.z);
		}
		/*!
		*@brief	Effekseer::Manager�̎擾�B
		*/
		Effekseer::Manager* GetManager()
		{
			return m_effekseerManager;
		}
		/*!
		*@brief	�C���X�^���X�̎擾�B
		*/
		static EffekseerManager& Instance()
		{
			static EffekseerManager instance;//EffekseerManager�̃C���X�^���X�𐶐��B
			return instance;
		}
	private:
		//Effekseer�}�l�[�W���Ǘ��B
		Effekseer::Manager*	m_effekseerManager = nullptr;
		EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;
		bool initflag = false;      //��������������ێ�����B
	};

	//Player_BulletManager�̃C���X�^���X���擾�B
	static inline EffekseerManager& G_EffekseerManager()
	{
		return EffekseerManager::Instance();
	}
}