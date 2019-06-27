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
	enum AnimClip {//�e�A�j���[�V�����̃N���b�v�B
		HatchOpen,       //�J��
		HatchClose,      //����
		Animnum           //AnimationClip�̑���
	};
	Animation GL_Animation;				//�A�j���[�V�����B
	AnimationClip GL_AnimationClips[Animnum];	//�A�j���[�V�����N���b�v�B
};

