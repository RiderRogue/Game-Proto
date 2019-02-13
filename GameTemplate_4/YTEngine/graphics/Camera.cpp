#include "stdafx.h"
#include "Camera.h"
namespace YTEngine {
	Camera g_camera3D;		//3Dカメラ。
	Camera g_camera2D;		//2Dカメラ。

	void Camera::Update()
	{
		//ビュー行列を計算。
		m_viewMatrix.MakeLookAt(
			m_position,
			m_target,
			m_up
		);
		//プロジェクション行列を計算
		if (m_updateProjMatrixFunc==enUpdateProjMatrixFunc_Perspective) {
			//透視変換行列を計算。
			m_projMatrix.MakeProjectionMatrix(
				m_viewAngle,					//画角。
				FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
				m_near,
				m_far
			);
		}
		else {
			//平行投影行列を計算。
			m_projMatrix.MakeOrthoProjectionMatrix(
				FRAME_BUFFER_W,
				FRAME_BUFFER_H,
				m_near,
				m_far
			);
		}
	}
}