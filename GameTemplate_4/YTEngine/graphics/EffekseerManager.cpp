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
		//�����_���[���������B
		m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			YTEngine::g_graphicsEngine->GetD3DDevice(),			//D3D�f�o�C�X�B
			YTEngine::g_graphicsEngine->GetD3DDeviceContext(),	//D3D�f�o�C�X�R���e�L�X�g�B
			20000										//�|���̍ő吔�B
		);
		//�G�t�F�N�g�}�l�[�W�����������B
		m_effekseerManager = Effekseer::Manager::Create(10000);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
		m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
		m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
		m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
		m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
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
		//Effekseer�J�����s���ݒ�B
		//�܂���Effeseer�̍s��^�̕ϐ��ɁA�J�����s��ƃv���W�F�N�V�����s����R�s�[�B
		Effekseer::Matrix44 efCameraMat;
		YTEngine::g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		YTEngine::g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		//�J�����s��ƃv���W�F�N�V�����s���ݒ�B
		m_effekseerRenderer->SetCameraMatrix(efCameraMat);
		m_effekseerRenderer->SetProjectionMatrix(efProjMat);
		//Effekseer���X�V�B
		m_effekseerManager->Update();
	}

	void EffekseerManager::Draw()
	{
		if (initflag == true) {
			//�G�t�F�N�g�͕s�����I�u�W�F�N�g��`�悵����ŕ`�悷��B
			m_effekseerRenderer->BeginRendering();
			m_effekseerManager->Draw();
			m_effekseerRenderer->EndRendering();
		}
	}
}