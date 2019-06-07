#pragma once
using namespace YTEngine;
#include "Level.h"
class Stage_Base : public IGameObject
{
public:
	Stage_Base();
	virtual~Stage_Base();
	virtual bool Start();
	virtual void Update();
	virtual void Draw();
	virtual void OnDestroy();
protected:
	Level * level;
	SkinModel m_model;//ãÛÅB
};

