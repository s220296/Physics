#pragma once

#include "GameScene.h"
#include "glm/vec2.hpp"
#include "Input.h"

#include <vector>
#include <string>

class PhysicsObject;
class PhysicsScene;
class Circle;
class Spring;

class GamePlayScene : public GameScene
{
public:
	GamePlayScene();
	
	void Enter() override;
	void Update(float dt) override;
	void Draw() override;

	void GenerateLevel();
	std::vector<std::string> GetLevel(int level);

protected:
	std::vector<PhysicsObject*> m_levelObjects;

	PhysicsScene* m_physicsScene;

	Circle* m_targetBody;

	Circle* m_player;
	Spring* m_grapple;

	bool isGrappling;

	glm::vec2 grapplePoint;
	aie::Input* m_input;

	int m_level;
};

