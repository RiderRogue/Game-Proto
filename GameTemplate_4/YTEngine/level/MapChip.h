#pragma once

#include "physics/PhysicsStaticObject.h"

namespace YTEngine {
class Level;
struct LevelObjectData;

	/*!
	* @brief	�}�b�v�`�b�v�B
	*/
	class MapChip {
	public:
		/*!
		* @brief	�R���X�g���N�^�B
		*@param[in] objData			�I�u�W�F�N�g���B
		*/
		MapChip(const LevelObjectData& objData);
		/*!
		* @brief	�`��B
		*/
		void Draw();

		/*!
		*@brief �}�b�v�`�b�v���V���h�E�L���X�^�[�֓o�^�B
		*/
		void SetShadowCasters();
	private:
		SkinModel m_model;		//!<���f���B
		PhysicsStaticObject m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g�B

	};
}