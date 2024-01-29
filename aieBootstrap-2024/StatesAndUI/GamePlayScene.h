#pragma once

#include "GameScene.h"

class GamePlayScene : public GameScene
{
public:
	GamePlayScene();
	
	void Update(float dt) override;
	void Draw() override;
};

