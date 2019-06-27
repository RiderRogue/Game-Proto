#pragma once
#include "Enemy.h"
class GroundingLauncher :
	public Enemy
{
public:
	GroundingLauncher();
	~GroundingLauncher();
	bool Start();
	void Update();
	void PostDraw();
private:
	enum AnimClip {//各アニメーションのクリップ。
		HatchOpen,       //開く
		HatchClose,      //閉じる
		Animnum           //AnimationClipの総数
	};
	Animation GL_Animation;				//アニメーション。
	AnimationClip GL_AnimationClips[Animnum];	//アニメーションクリップ。
};

