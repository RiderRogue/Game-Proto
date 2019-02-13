#pragma once
/*!
* @brief	NoncopyÉ|ÉäÉVÅ[
*/
namespace YTEngine {
	struct Noncopyable {
		Noncopyable() = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}