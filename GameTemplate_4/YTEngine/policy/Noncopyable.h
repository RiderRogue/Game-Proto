#pragma once
/*!
* @brief	Noncopy�|���V�[
*/
namespace YTEngine {
	struct Noncopyable {
		Noncopyable() = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};
}