#pragma once
namespace YTEngine {
	class EffekseerManager
	{
	public:
		EffekseerManager();
		~EffekseerManager();
		/*!
		*@brief	初期化。
		*/
		void Init();

		void Update();
		void Draw();
		void OnDestroy();

		//エフェクトの再生。
		Effekseer::Handle Play(Effekseer::Effect* effect, CVector3 EffectPos)
		{
			return m_effekseerManager->Play(effect, EffectPos.x, EffectPos.y, EffectPos.z);
		}
		/*Effekseer::Handle Play(Effekseer::Effect* effect, float x, float y, float z)
		{
			return m_effekseerManager->Play(effect, x, y, z);
		}*/
		//再生中のエフェクトを止める。
		void StopEffect(Effekseer::Handle handle)
		{
			m_effekseerManager->StopEffect(handle);
		}

		//再生中のエフェクトの座標を設定。
		void SetEffectposition(Effekseer::Handle handle,CVector3 pos)
		{
			m_effekseerManager->SetLocation(handle, pos.x, pos.y, pos.z);
		}
		/*!
		*@brief	Effekseer::Managerの取得。
		*/
		Effekseer::Manager* GetManager()
		{
			return m_effekseerManager;
		}
		/*!
		*@brief	インスタンスの取得。
		*/
		static EffekseerManager& Instance()
		{
			static EffekseerManager instance;//EffekseerManagerのインスタンスを生成。
			return instance;
		}
	private:
		//Effekseerマネージャ管理。
		Effekseer::Manager*	m_effekseerManager = nullptr;
		EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;
		bool initflag = false;      //初期化したかを保持する。
	};

	//Player_BulletManagerのインスタンスを取得。
	static inline EffekseerManager& G_EffekseerManager()
	{
		return EffekseerManager::Instance();
	}
}