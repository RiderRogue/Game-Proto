#pragma once
using namespace YTEngine;
#include "../title.h"
#include "../MainGame.h"
#include "../Stageselect.h"
class GameBase : public IGameObject
{
public:
	enum GameBaseState
	{
		GameBase_title,
		GameBase_stageselect,
		GameBase_mainGame
	};
	GameBase();
	~GameBase();
	bool Start();
	void Update();
	void ChangeScene(GameBaseState state);

	void SetStage_num(int num) {
		stage_num = num;
	}

	int GetStage_num() {
		return stage_num;
	}
private:
	int stage_num;//ステージの番号。
	title* b_title=nullptr;
	Stageselect* b_stageselect = nullptr;
	MainGame* b_maingame = nullptr;
};
