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

	void HP_meter(float hp, float hpMAX)
	{
		e_hp = hp / hpMAX;
	}
	void Energy_meter(float energy, float energyMAX)
	{
		e_gauge = energy / energyMAX;
	}
private:
	Sprite m_HP_frame;
	Sprite m_HP_meter;
	Sprite m_energy_frame;
	Sprite m_energy_meter;
	float e_hp = 1.0f;
	float e_gauge = 1.0f;
};

