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
		//静的物理オブジェクトをメッシュコライダーから作成する。
		m_physicsStaticObject.CreateMeshObject(m_model, objData.position, objData.rotation);
		//地面をシャドウレシーバーにする。
		m_model.SetShadowReciever(true);
	}

	void MapChip::Draw()
	{
		m_model.Draw(enRenderMode_Normal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}

	void MapChip::SetShadowCasters()
	{
		//シャドウキャスターを登録。																			 //シャドウキャスターを登録。
		Shadow_map().RegistShadowCaster(&m_model);
		m_model.SetShadowReciever(true);
	}
}