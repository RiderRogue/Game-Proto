/*!
* @brief	コリジョン属性
*/
#pragma once

/*!
* @brief	コリジョン属性の大分類。
*/
enum EnCollisionAttr {
	enCollisionAttr_Ground,
	enCollisionAttr_Character,
	enCollisionAttr_User,		//以下にユーザー定義のコリジョン属性を設定する。
	enCollisionAttr_Stage,
	enCollisionAttr_Player,
	enCollisionAttr_PlayerBullet,
	enCollisionAttr_Enemy,
	enCollisionAttr_EnemyBullet,
	enCollisionAttr_EnemyMissile
};