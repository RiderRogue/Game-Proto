#include "stdafx.h"
#include "GameObjectManager.h"


namespace YTEngine {
	CGameObjectManager::CGameObjectManager() :
		m_gameObjectPriorityMax(0)
	{
		//メインとなるレンダリングターゲットを作成する。
		m_mainRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);

		//メインレンダリングターゲットに描かれた絵を
		//フレームバッファにコピーするためのスプライトを初期化する。
		m_copyMainRtToFrameBufferSprite.Init(
			m_mainRenderTarget.GetRenderTargetSRV(),
			FRAME_BUFFER_W,
			FRAME_BUFFER_H
		);
	}

	void CGameObjectManager::Execute()
	{
		///////////////////////////////////////////////
		//シャドウマップにレンダリング
		///////////////////////////////////////////////
		auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//現在のレンダリングターゲットをバックアップしておく。
		d3dDeviceContext->OMGetRenderTargets(
			1,
			&m_frameBufferRenderTargetView,
			&m_frameBufferDepthStencilView
		);
		//ビューポートもバックアップを取っておく。
		unsigned int numViewport = 1;
		d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

		//始めに前フレームでいらなくなったIGameObjectを削除。
		ExecuteDeleteGameObjects();

		//Start処理。
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
		
		//IGameObjectのアップデート
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
		//シャドウマップにレンダリング
		Shadow_map().RenderToShadowMap();

		//レンダリングターゲットをメインに変更する。
		g_graphicsEngine->ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);
		//メインレンダリングターゲットをクリアする。
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_mainRenderTarget.ClearRenderTarget(clearColor);
		/*//元に戻す。
		d3dDeviceContext->OMSetRenderTargets(
			1,
			&m_frameBufferRenderTargetView,
			m_frameBufferDepthStencilView
		);
		d3dDeviceContext->RSSetViewports(numViewport, &m_frameBufferViewports);*/
		
		//IGameObjectの描画
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->DrawWrapper();
			}
		}

		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
		//ポストエフェクトの描画。
		m_postEffect.Draw();

		//レンダリングターゲットをフレームバッファに戻す。
		g_graphicsEngine->ChangeRenderTarget(
			m_frameBufferRenderTargetView,
			m_frameBufferDepthStencilView,
			&m_frameBufferViewports
		);

		m_copyMainRtToFrameBufferSprite.Draw();

		//レンダリングターゲットとデプスステンシルの参照カウンタを下す。
		m_frameBufferRenderTargetView->Release();
		m_frameBufferDepthStencilView->Release();
		//シーングラフを更新。
		UpdateSceneGraph();
		
	}

	//必要なオブジェクトをシャドウキャスターに登録する。
	void CGameObjectManager::SetObjectShadowCasters()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->SetShadowCastersWrapper();
			}
		}
	}

	void CGameObjectManager::UpdateSceneGraph()
	{
		//ワールド行列を更新。
		for (auto transform : m_childrenOfRootTransformList) {
			transform->UpdateWorldMatrixAll();
		}
	}
	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//バッファを切り替え。
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
			for (IGameObject* go : goList) {
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find(goExecList.begin(), goExecList.end(), go);
				if (it != goExecList.end()) {
					//削除リストから除外された。
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManager()) {
						delete (*it);
					}
				}
				goExecList.erase(it);
			}
			goList.clear();
		}
	}
	void CGameObjectManager::Init(int gameObjectPrioMax)
	{
		//psShader.Load("")
		//TK_ASSERT(gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX, "ゲームオブジェクトの優先度の最大数が大きすぎます。");
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
}