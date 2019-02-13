#include "stdafx.h"
#include "Camera.h"
namespace YTEngine {
	Camera g_camera3D;		//3D�J�����B
	Camera g_camera2D;		//2D�J�����B

	void Camera::Update()
	{
		//�r���[�s����v�Z�B
		m_viewMatrix.MakeLookAt(
			m_position,
			m_target,
			m_up
		);
		//�v���W�F�N�V�����s����v�Z
		if (m_updateProjMatrixFunc==enUpdateProjMatrixFunc_Perspective) {
			//�����ϊ��s����v�Z�B
			m_projMatrix.MakeProjectionMatrix(
				m_viewAngle,					//��p�B
				FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
				m_near,
				m_far
			);
		}
		else {
			//���s���e�s����v�Z�B
			m_projMatrix.MakeOrthoProjectionMatrix(
				FRAME_BUFFER_W,
				FRAME_BUFFER_H,
				m_near,
				m_far
			);
		}
	}
}