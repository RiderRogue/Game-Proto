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
	Sprite m_title;
	CVector3 m_position = CVector3::Zero();
};

