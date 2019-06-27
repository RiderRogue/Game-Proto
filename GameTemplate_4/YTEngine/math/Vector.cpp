/*!
* @brief	ベクトルクラス。
*/

#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"


namespace YTEngine {
	/*!
	*@brief	行列からクォータニオンを作成。
	*/
	void CQuaternion::SetRotation(const CMatrix& m)
	{
		DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
	}

	CVector3 CVector3::calcWallScratchVector(const CVector3& front, const CVector3& normal) {
		CVector3 front_f = front;
		CVector3 normal_n = normal;

		normal_n.Normalize();//正規化。
		float InverseDot = front_f.Dot(normal_n);//進行ベクトルの逆ベクトルを法線に投影
		front_f = front_f - (normal_n*InverseDot);
		front_f.Normalize();
		return front_f;
	}

	CVector3 CVector3::calcReflectVector(const CVector3& front, const CVector3& normal) {
		CVector3 front_f = front;
		CVector3 normal_n = normal;

		normal_n.Normalize();//正規化。
		float InverseDot = front_f.Dot(normal_n);//進行ベクトルの逆ベクトルを法線に投影
		InverseDot = InverseDot * 2.0f;//反射
		front_f = front_f - (normal_n*InverseDot);
		front_f.Normalize();
		return front_f;
	}
}