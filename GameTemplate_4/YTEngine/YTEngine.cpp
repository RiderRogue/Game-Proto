// YTEngine.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "YTEngine.h"

namespace YTEngine {

	CEngine::CEngine()
	{

	}
	CEngine::~CEngine()
	{
		Final();
	}

	bool CEngine::Init(int gameObjectPrioMax)
	{
		//GameObjectManagerの初期化。
		GameObjectManager().Init(gameObjectPrioMax);
		return true;
	}
	void CEngine::RunGameLoop()
	{
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//更新。ゲームループの処理が入っている。

				Update();
			}
		}
	}

	//主なゲームループの処理が入っている。
	void CEngine::Update()
	{
		
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		
		//物理エンジンの更新。
		g_physics.Update();

		//シャドウキャスターを登録。
		GameObjectManager().SetObjectShadowCasters();

		//シャドウマップを更新。
		/*Shadow_map().UpdateFromLightTarget(
			{ 1000.0f, 1000.0f, 1000.0f },
			{ 0.0f, 0.0f, 0.0f }
		);*/
		//描画開始。
		g_graphicsEngine->BegineRender();
		



		///////////////////////////////////////////////
		//ここから通常レンダリング。
		///////////////////////////////////////////////





		//GameObjectManager更新
		GameObjectManager().Execute();
		
		

		//描画終了。
		g_graphicsEngine->EndRender();

		
	}

	void CEngine::Final()
	{

		g_physics.Release();
		
		g_graphicsEngine->Release();
	}
}
