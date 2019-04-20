#pragma once
using namespace YTEngine;

class title : public IGameObject
{
public:
	title();
	~title();
	bool Start();
	void Update();
	void PostDraw();
private:
	Sprite m_titlebase;
	Sprite m_title01;
	Sprite m_title02;
	CVector3 m_position_base = CVector3::Zero();
	CVector3 m_position01 = CVector3::Zero();
	CVector3 m_position02 = CVector3::Zero();
	bool title_startflag = false;
};

