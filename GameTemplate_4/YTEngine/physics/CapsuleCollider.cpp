/*!
* @brief	カプセルコライダー。
*/

#include "stdafx.h"
#include "Physics/CapsuleCollider.h"


namespace YTEngine {
	/*!
	* @brief	コンストラクタ。
	*/
	CapsuleCollider::CapsuleCollider() :
		shape(nullptr)
	{
	}
	/*!
	* @brief	デストラクタ。
	*/
	CapsuleCollider::~CapsuleCollider()
	{
		delete shape;
	}
}