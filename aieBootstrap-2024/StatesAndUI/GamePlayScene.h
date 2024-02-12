#pragma once

#include "GameScene.h"
#include "glm/vec2.hpp"
#include "Input.h"

#include <vector>

class PhysicsObject;
class PhysicsScene;
class Circle;

class GamePlayScene : public GameScene
{
public:
	GamePlayScene();
	
	void Enter() override;
	void Update(float dt) override;
	void Draw() override;

	std::vector<PhysicsObject*> GenerateLevel();

protected:
	PhysicsScene* m_physicsScene;

	Circle* m_player;
	glm::vec2 grapplePoint;
	aie::Input* m_input;
};

