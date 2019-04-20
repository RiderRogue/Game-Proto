#include "stdafx.h"
#include "gauge.h"


gauge::gauge()
{
}


gauge::~gauge()
{
}

bool gauge::Start()
{
	m_HP_frame.Init(L"Resource/sprite/gauge_frame.dds", 600.0f, 100.0f);
	m_HP_frame.Update({ 40.0f,320.0f,0.0f }, CQuaternion::Identity(), { 4.0f,0.5f,1.0f }, { 0.0f,0.0f });
	m_HP_meter.Init(L"Resource/sprite/gauge_meter.dds", 590.0f, 81.0f);
	m_HP_meter.Update({ 46.0f,316.0f,0.0f }, CQuaternion::Identity(), { 1.0f,1.0f,1.0f }, { 0.0f,0.0f });

	m_energy_frame.Init(L"Resource/sprite/Energy_frame.dds", 50.0f, 600.0f);
	m_energy_frame.Update({ -635.0f,-350.0f,0.0f }, CQuaternion::Identity(), { 4.0f,0.5f,1.0f }, { 0.0f,1.0f });
	m_energy_meter.Init(L"Resource/sprite/Energy_meter.dds", 40.0f, 590.0f);
	m_energy_meter.Update({ -630.0f,-345.0f,0.0f }, CQuaternion::Identity(), { 1.0f,1.0f,1.0f }, { 0.0f,1.0f });
	return true;
}

void gauge::Update()
{
	m_HP_frame.Update({ 40.0f,310.0f,0.0f }, CQuaternion::Identity(), { 1.0f,0.5f,1.0f }, { 0.0f,0.0f });
	m_HP_meter.Update({ 46.0f,314.0f,0.0f }, CQuaternion::Identity(), { e_hp,0.5f,1.0f }, { 0.0f,0.0f });
	
	m_energy_frame.Update({ -635.0f,-350.0f,0.0f }, CQuaternion::Identity(), { 1.0f,1.0f,1.0f }, { 0.0f,0.0f });
	m_energy_meter.Update({ -630.0f,-345.0f,0.0f }, CQuaternion::Identity(), { 1.0,e_gauge,1.0f }, { 0.0f,0.0f });

}

void gauge::HP_meter(float hp,float hpMAX)
{
	e_hp = hp / hpMAX;
}

void gauge::Energy_meter(float energy,float energyMAX)
{
	e_gauge = energy / energyMAX;
}

void gauge::PostDraw()
{
	m_HP_frame.Draw();
	m_HP_meter.Draw();
	m_energy_frame.Draw();
	m_energy_meter.Draw();
}