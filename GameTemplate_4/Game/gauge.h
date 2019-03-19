#pragma once
using namespace YTEngine;
class gauge : public IGameObject
{
public:
	gauge();
	~gauge();
	bool Start();
	void Update();
	void PostDraw();
private:
	Sprite m_gauge_frame;
	Sprite m_gauge_meter;
	float s_x = 4.0f;
};

