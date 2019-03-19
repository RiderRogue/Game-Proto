#include "stdafx.h"
#include "title.h"
#include "GameBase/GameBase.h"

using namespace YTEngine;
title::title()
{
}


title::~title()
{
}

bool title::Start()
{
	m_title.Init(L"Resource/sprite/konkurito.dds", 1280.0f, 720.0f);
	m_title.Update(m_position, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	return true;
}

void title::Update()
{
	m_title.Update(m_position, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	if (g_pad[0].IsPress(enButtonA))
	{
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_mainGame);
	}
	m_position.y += 1.0f;
	if (m_position.y > 150.0f) {
		FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_mainGame);
	}
	
}

void title::PostDraw()
{
	m_title.Draw();
}