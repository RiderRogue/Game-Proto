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
	m_gauge_frame.Init(L"Resource/sprite/gauge_frame.dds", 150.0f, 80.0f);
	m_gauge_frame.Update({ 40.0f,320.0f,0.0f }, CQuaternion::Identity(), { 4.0f,0.5f,1.0f }, { 0.0f,0.0f });
	m_gauge_meter.Init(L"Resource/sprite/gauge_meter.dds", 147.0f, 65.0f);
	m_gauge_meter.Update({ 46.0f,316.0f,0.0f }, CQuaternion::Identity(), { 1.0f,1.0f,1.0f }, { 0.0f,0.0f });
	return true;
}

void gauge::Update()
{
	m_gauge_frame.Update({ 40.0f,320.0f,0.0f }, CQuaternion::Identity(), { 4.0f,0.5f,1.0f }, { 0.0f,0.0f });
	m_gauge_meter.Update({ 46.0f,323.0f,0.0f }, CQuaternion::Identity(), { s_x,0.5f,1.0f }, { 0.0f,0.0f });
	s_x -= 0.01f;
}

void gauge::PostDraw()
{
	m_gauge_frame.Draw();
	m_gauge_meter.Draw();
}