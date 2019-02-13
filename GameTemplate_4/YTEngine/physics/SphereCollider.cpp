/*!
* @brief	スフィアコライダー。
*/

#include "stdafx.h"
#include "Physics/SphereCollider.h"

namespace YTEngine {
	/*!
	* @brief	コンストラクタ。
	*/
	SphereCollider::SphereCollider():
		shape(nullptr)
	{
	}

	/*!
	* @brief	デストラクタ。
	*/
	SphereCollider::~SphereCollider()
	{
		delete shape;
	}
}