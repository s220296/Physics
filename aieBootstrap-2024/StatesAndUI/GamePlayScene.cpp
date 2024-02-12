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

	m_targetBody = new Circle(vec2(40, 40), vec2(0), 5.f, 3.5f, vec4(1, 0, 0, 1));

	m_targetBody->collisionCallback = [=](PhysicsObject* other)
		{ 
			Circle* circle = dynamic_cast<Circle*>(other);
			if (circle && circle == m_player)
			{
				// what happens when target is hit?
				// probably just win
				
			}
		};

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetCurrentInstance(m_physicsScene);
	m_physicsScene->SetGravity(vec2(0, -18));
	m_physicsScene->SetTimeStep(0.01f);

	m_physicsScene->AddActor(m_player);
	m_physicsScene->AddActor(m_targetBody);

	std::vector<PhysicsObject*> levelObjects = GenerateLevel();

	for (PhysicsObject* po : levelObjects)
		m_physicsScene->AddActor(po);

	grapplePoint = glm::vec2(0);
	isGrappling = false;
}

void GamePlayScene::Update(float dt)
{
	m_physicsScene->Update(dt);

	if (m_input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) && !isGrappling)
	{
		glm::vec2 pointOfContact = glm::vec2(0);

		PhysicsObject* po = m_physicsScene->LineCast(m_player, // ignore the player
			m_player->GetPosition(), // cast from player
			StatesAndUIApp::worldMousePos - m_player->GetPosition(), // cast towards mouse
			glm::distance(StatesAndUIApp::worldMousePos, m_player->GetPosition()), 
			pointOfContact);

		if (po) // if legal grapple point, commence grapple
		{
			grapplePoint = pointOfContact;

			m_grapple = new Spring(m_player, nullptr, 2.5f, 0.f, 0.3f, glm::vec2(0), grapplePoint);
			m_physicsScene->AddActor(m_grapple);

			isGrappling = true;
		}
	}
	else if(m_input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT) && isGrappling)
	{
		grapplePoint = glm::vec2(0);
		m_physicsScene->RemoveActor(m_grapple);

		isGrappling = false;

		delete m_grapple;
		m_grapple = nullptr;
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
	sLevel.push_back("10000000000013010001");
	sLevel.push_back("10000000000020010001");
	sLevel.push_back("10000000000020010001");
	sLevel.push_back("10000000000011110001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10010000000000000001");
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

			box->SetAngularDrag(8.f); box->SetLinearDrag(2.f); box->SetElasticity(0.6f);
		}
		if (level[i] == 2)
		{
			Box* box = new Box(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)),
				glm::vec2(0), 5.f, glm::vec2(blockSize.y * 0.5f), 0.f, glm::vec4(1, 1, 0, 1));
			box->SetKinematic(false);
			result.push_back(box);

			box->SetAngularDrag(8.f); box->SetLinearDrag(2.f); box->SetElasticity(0.4f);
		}
		if (level[i] == 3)
		{
			m_targetBody->SetPosition(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)));
		}
	}

	delete[] level;

	return result;
}