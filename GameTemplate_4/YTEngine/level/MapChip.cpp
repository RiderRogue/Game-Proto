#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

namespace YTEngine {
	MapChip::MapChip(const LevelObjectData& objData)
	{
		wchar_t filePath[256];
		swprintf_s(filePath, L"Assets/modelData/%s.cmo", objData.name);
		m_model.Init(filePath);
		m_model.UpdateWorldMatrix(objData.position, objData.rotation, CVector3::One());
		//�ÓI�����I�u�W�F�N�g�����b�V���R���C�_�[����쐬����B
		m_physicsStaticObject.CreateMeshObject(m_model, objData.position, objData.rotation);
		//�n�ʂ��V���h�E���V�[�o�[�ɂ���B
		m_model.SetShadowReciever(true);
	}

	void MapChip::Draw()
	{
		m_model.Draw(enRenderMode_Normal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}

	void MapChip::SetShadowCasters()
	{
		//�V���h�E�L���X�^�[��o�^�B																			 //�V���h�E�L���X�^�[��o�^�B
		Shadow_map().RegistShadowCaster(&m_model);
		m_model.SetShadowReciever(true);
	}
}