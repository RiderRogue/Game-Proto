#include "stdafx.h"
#include "GameObjectManager.h"


namespace YTEngine {
	CGameObjectManager::CGameObjectManager() :
		m_gameObjectPriorityMax(0)
	{
		//���C���ƂȂ郌���_�����O�^�[�Q�b�g���쐬����B
		m_mainRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			DXGI_FORMAT_R16G16B16A16_FLOAT
		);

		//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G��
		//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������B
		m_copyMainRtToFrameBufferSprite.Init(
			m_mainRenderTarget.GetRenderTargetSRV(),
			FRAME_BUFFER_W,
			FRAME_BUFFER_H
		);
	}

	void CGameObjectManager::Execute()
	{
		///////////////////////////////////////////////
		//�V���h�E�}�b�v�Ƀ����_�����O
		///////////////////////////////////////////////
		auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
		d3dDeviceContext->OMGetRenderTargets(
			1,
			&m_frameBufferRenderTargetView,
			&m_frameBufferDepthStencilView
		);
		//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
		unsigned int numViewport = 1;
		d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

		//�n�߂ɑO�t���[���ł���Ȃ��Ȃ���IGameObject���폜�B
		ExecuteDeleteGameObjects();

		//Start�����B
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
		
		//IGameObject�̃A�b�v�f�[�g
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
		//�V���h�E�}�b�v�Ƀ����_�����O
		Shadow_map().RenderToShadowMap();

		//�����_�����O�^�[�Q�b�g�����C���ɕύX����B
		g_graphicsEngine->ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);
		//���C�������_�����O�^�[�Q�b�g���N���A����B
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_mainRenderTarget.ClearRenderTarget(clearColor);
		/*//���ɖ߂��B
		d3dDeviceContext->OMSetRenderTargets(
			1,
			&m_frameBufferRenderTargetView,
			m_frameBufferDepthStencilView
		);
		d3dDeviceContext->RSSetViewports(numViewport, &m_frameBufferViewports);*/
		
		//IGameObject�̕`��
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
		//�|�X�g�G�t�F�N�g�̕`��B
		m_postEffect.Draw();

		//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
		g_graphicsEngine->ChangeRenderTarget(
			m_frameBufferRenderTargetView,
			m_frameBufferDepthStencilView,
			&m_frameBufferViewports
		);

		m_copyMainRtToFrameBufferSprite.Draw();

		//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�ƃJ�E���^�������B
		m_frameBufferRenderTargetView->Release();
		m_frameBufferDepthStencilView->Release();
		//�V�[���O���t���X�V�B
		UpdateSceneGraph();
		
	}

	//�K�v�ȃI�u�W�F�N�g���V���h�E�L���X�^�[�ɓo�^����B
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
		//���[���h�s����X�V�B
		for (auto transform : m_childrenOfRootTransformList) {
			transform->UpdateWorldMatrixAll();
		}
	}
	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//�o�b�t�@��؂�ւ��B
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
			for (IGameObject* go : goList) {
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find(goExecList.begin(), goExecList.end(), go);
				if (it != goExecList.end()) {
					//�폜���X�g���珜�O���ꂽ�B
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
		//TK_ASSERT(gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
}