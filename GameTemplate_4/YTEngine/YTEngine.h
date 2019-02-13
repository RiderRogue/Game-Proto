#pragma once
#include "ShadowMap.h"
namespace YTEngine {
	/*!
	*@brief	エンジン。
	*/
	class CEngine : Noncopyable {
	private:
		/*!
		*@brief	コンストラクタ。
		*/
		CEngine();
		/*!
		*@brief	デストラクタ。
		*/
		~CEngine();
	public:
		/*!
		*@brief	初期化。
		*/
		bool Init(int gameObjectPrioMax);
		/*!
		*@brief	ゲームループを実行。
		*/
		void RunGameLoop();
		/*!
		* @brief	更新。
		*/
		void Update();
		/*!
		*@brief	終了処理。
		*/
		void Final();
		/*!
		*@brief	インスタンスを取得。
		*/
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;//エンジンのインスタンスを作成。
			}
			return *instance;
		}

		/// <summary>
		/// シャドウマップを取得。
		/// </summary>
		/// <returns></returns>
		ShadowMap* GetShadowMap()
		{
			return &m_shadowMap;
		}
		
	private:
		
		ShadowMap m_shadowMap;					//シャドウマップ。
		
	};

	/*!
	* @brief	Engineのインスタンスを取得。
	*/
	static inline CEngine& Engine()
	{
		return CEngine::GetInstance();
	}

}