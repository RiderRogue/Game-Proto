/*!
* @brief	�X�t�B�A�R���C�_�[�B
*/

#include "stdafx.h"
#include "Physics/SphereCollider.h"

namespace YTEngine {
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	SphereCollider::SphereCollider():
		shape(nullptr)
	{
	}

	/*!
	* @brief	�f�X�g���N�^�B
	*/
	SphereCollider::~SphereCollider()
	{
		delete shape;
	}
}