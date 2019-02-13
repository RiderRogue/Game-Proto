
/*!
* @brief	コライダー。
*/

#pragma once

class btCollisionShape;
namespace YTEngine {
	/*!
	* @brief	コライダーのインターフェースクラス。
	*/
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() const = 0;
	};
}