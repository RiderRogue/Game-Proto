/*!
* @brief	�x�N�g���N���X�B
*/

#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"


namespace YTEngine {
	/*!
	*@brief	�s�񂩂�N�H�[�^�j�I�����쐬�B
	*/
	void CQuaternion::SetRotation(const CMatrix& m)
	{
		DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
	}

	CVector3 CVector3::calcWallScratchVector(const CVector3& front, const CVector3& normal) {
		CVector3 front_f = front;
		CVector3 normal_n = normal;

		normal_n.Normalize();//���K���B
		float InverseDot = front_f.Dot(normal_n);//�i�s�x�N�g���̋t�x�N�g����@���ɓ��e
		front_f = front_f - (normal_n*InverseDot);
		front_f.Normalize();
		return front_f;
	}

	CVector3 CVector3::calcReflectVector(const CVector3& front, const CVector3& normal) {
		CVector3 front_f = front;
		CVector3 normal_n = normal;

		normal_n.Normalize();//���K���B
		float InverseDot = front_f.Dot(normal_n);//�i�s�x�N�g���̋t�x�N�g����@���ɓ��e
		InverseDot = InverseDot * 2.0f;//����
		front_f = front_f - (normal_n*InverseDot);
		front_f.Normalize();
		return front_f;
	}
}