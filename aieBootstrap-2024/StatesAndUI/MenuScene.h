#pragma once

#include "GameScene.h"

class MenuScene : public GameScene
{
public:
	MenuScene();

	void Update(float dt) override;
	void Draw() override;
};

