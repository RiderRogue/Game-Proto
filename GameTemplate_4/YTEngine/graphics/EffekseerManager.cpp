#include "stdafx.h"
#include "EffekseerManager.h"

namespace YTEngine {
	EffekseerManager::EffekseerManager()
	{
	}


	EffekseerManager::~EffekseerManager()
	{
	}

	void EffekseerManager::Init()
	{
		//レンダラーを初期化。
		m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			YTEngine::g_graphicsEngine->GetD3DDevice(),			//D3Dデバイス。
			YTEngine::g_graphicsEngine->GetD3DDeviceContext(),	//D3Dデバイスコンテキスト。
			20000										//板ポリの最大数。
		);
		//エフェクトマネージャを初期化。
		m_effekseerManager = Effekseer::Manager::Create(10000);

		// 描画用インスタンスから描画機能を設定
		m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
		m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
		m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
		m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
		m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
		m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());

		initflag = true;
	}

	void EffekseerManager::OnDestroy()
	{
		if (m_effekseerManager != nullptr) {
			m_effekseerManager->Destroy();

		}
		if (m_effekseerRenderer != nullptr) {
			m_effekseerRenderer->Destroy();
		}

		initflag = false;
	}

	void EffekseerManager::Update()
	{
		//Effekseerカメラ行列を設定。
		//まずはEffeseerの行列型の変数に、カメラ行列とプロジェクション行列をコピー。
		Effekseer::Matrix44 efCameraMat;
		YTEngine::g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		YTEngine::g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		//カメラ行列とプロジェクション行列を設定。
		m_effekseerRenderer->SetCameraMatrix(efCameraMat);
		m_effekseerRenderer->SetProjectionMatrix(efProjMat);
		//Effekseerを更新。
		m_effekseerManager->Update();
	}

	void EffekseerManager::Draw()
	{
		if (initflag == true) {
			//エフェクトは不透明オブジェクトを描画した後で描画する。
			m_effekseerRenderer->BeginRendering();
			m_effekseerManager->Draw();
			m_effekseerRenderer->EndRendering();
		}
	}
}