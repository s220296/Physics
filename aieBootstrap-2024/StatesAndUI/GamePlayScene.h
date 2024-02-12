#pragma once

#include "GameScene.h"

class PhysicsScene;

class GamePlayScene : public GameScene
{
public:
	GamePlayScene();
	
	void Enter() override;
	void Update(float dt) override;
	void Draw() override;

protected:
	PhysicsScene* m_physicsScene;

};

