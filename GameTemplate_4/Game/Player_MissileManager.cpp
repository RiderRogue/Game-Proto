#include "stdafx.h"
#include "Player_MissileManager.h"


Player_MissileManager::Player_MissileManager()
{
}


Player_MissileManager::~Player_MissileManager()
{
}

void Player_MissileManager::Start()
{
}

void Player_MissileManager::Update()
{
}

void Player_MissileManager::missileLock(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward)
{
	//弾切れならロックオンしない。
	if (missile_ammo_NUM <= 0) {

	}
}

void Player_MissileManager::missileShot(const CVector3 p_position, const CVector3 p_forward, const CVector3 c_forward)
{
	//弾切れなら発射しない。
	if (missile_ammo_NUM <= 0) {
		
	}
}