#include "GamePlayScene.h"
#include "Gizmos.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Input.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Box.h"
#include "Spring.h"
#include "Application.h"
#include "StatesAndUIApp.h"

#include <string>

using glm::vec2;
using glm::vec4;

GamePlayScene::GamePlayScene() : GameScene("Gameplay")
{
}

void GamePlayScene::Enter()
{
	m_input = aie::Input::getInstance();
	
	m_player = new Circle(vec2(0), vec2(0), 5.f, 4.f, vec4(1));
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetCurrentInstance(m_physicsScene);
	m_physicsScene->SetGravity(vec2(0, -9));
	m_physicsScene->SetTimeStep(0.01f);

	m_physicsScene->AddActor(m_player);

	std::vector<PhysicsObject*> levelObjects = GenerateLevel();

	for (PhysicsObject* po : levelObjects)
		m_physicsScene->AddActor(po);
}

void GamePlayScene::Update(float dt)
{
	m_physicsScene->Update(dt);

	if (m_input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		PhysicsObject* po = m_physicsScene->PointCast(StatesAndUIApp::worldMousePos);

		if (po)// make this grapple point
			aie::Gizmos::add2DCircle(StatesAndUIApp::worldMousePos, 5.f, 15, vec4(1));
	}
}

void GamePlayScene::Draw()
{
	m_physicsScene->Draw();
}

std::vector<PhysicsObject*> GamePlayScene::GenerateLevel()
{
	std::vector<std::string> sLevel;

	// 20 x 20
	sLevel.push_back("11111111111111111111");
	sLevel.push_back("10001000000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000011110001");
	sLevel.push_back("10000000000010010001");
	sLevel.push_back("10000000000010010001");
	sLevel.push_back("10000000000000010001");
	sLevel.push_back("10000000000011110001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("11111111111111111111");

	int* level = new int[400];

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			level[i * sLevel[0].length() + j] = sLevel[i][j] - '0';
		}
	}

	std::vector<PhysicsObject*> result;
	glm::vec2 blockSize = glm::vec2(9, 5);

	for (int i = 0; i < 400; i++)
	{
		if (level[i] == 1)
		{
			Box* box = new Box(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)),
				glm::vec2(0), 5.f, blockSize * 0.5f, 0.f, glm::vec4(0, 1, 0, 1));
			box->SetKinematic(true);
			result.push_back(box);
		}
	}

	delete[] level;

	return result;
}