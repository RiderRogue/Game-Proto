#include "stdafx.h"
#include "title.h"
#include "GameBase/GameBase.h"
#include "graphics/EffekseerManager.h"

using namespace YTEngine;
title::title()
{
}


title::~title()
{
}

bool title::Start()
{
	//エフェクト関係の初期化。
	G_EffekseerManager().Init();
	m_position01.y = 100.0f;
	m_position02.y = (-100.0f);
	
	m_titlebase.Init(L"Resource/sprite/TitleBase.dds", 1280.0f, 720.0f);
	m_titlebase.Update(m_position_base, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	m_title01.Init(L"Resource/sprite/Title01.dds", 800.0f, 200.0f);
	m_title01.Update(m_position01, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	m_title02.Init(L"Resource/sprite/Title02.dds", 800.0f, 200.0f);
	m_title02.Update(m_position02, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	return true;
}

void title::Update()
{
	m_titlebase.Update(m_position_base, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	m_title01.Update(m_position01, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	m_title02.Update(m_position02, CQuaternion::Identity(), { 1.0f,1.0f,1.0f });
	if (g_pad[0].IsPress(enButtonA))
	{
		title_startflag = true;
	}
	
	if (title_startflag == true) {
		m_position01.y += 5.0f;
		m_position02.y -= 5.0f;
		if (m_position01.y>=450.0f) {
			FindGO<GameBase>("GameBase")->ChangeScene(GameBase::GameBase_stageselect);
			//DeleteGO(this);
		}
	}
	
}

void title::PostDraw()
{
	m_titlebase.Draw();
	m_title01.Draw();
	m_title02.Draw();
}