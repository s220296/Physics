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

	m_player = new Circle(vec2(0, 0), vec2(0), 5.f, 4.f, vec4(1));
	m_player->SetElasticity(0.7f);

	m_targetBody = new Circle(vec2(40, 40), vec2(0), 5.f, 3.5f, vec4(1, 0, 0, 1));

	m_targetBody->collisionCallback = [=](PhysicsObject* other)
		{ 
			Circle* circle = dynamic_cast<Circle*>(other);
			if (circle && circle == m_player)
			{
				GenerateLevel();

				grapplePoint = glm::vec2(0);
				m_physicsScene->RemoveActor(m_grapple);

				isGrappling = false;

				delete m_grapple;
				m_grapple = nullptr;
			}
		};

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetCurrentInstance(m_physicsScene);
	m_physicsScene->SetGravity(vec2(0, -18));
	m_physicsScene->SetTimeStep(0.01f);

	m_physicsScene->AddActor(m_player);
	m_physicsScene->AddActor(m_targetBody);

	m_level = 1;

	GenerateLevel();

	grapplePoint = glm::vec2(0);
	isGrappling = false;
}

void GamePlayScene::Update(float dt)
{
	m_physicsScene->Update(dt);
	// Try to engage grapple
	if (m_input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) && !isGrappling)
	{
		glm::vec2 pointOfContact = glm::vec2(0);

		PhysicsObject* po = m_physicsScene->LineCast(m_player, // ignore the player
			m_player->GetPosition(), // cast from player
			StatesAndUIApp::worldMousePos - m_player->GetPosition(), // cast towards mouse
			45.f, //glm::distance(StatesAndUIApp::worldMousePos, m_player->GetPosition()), 
			pointOfContact);

		if (po) // if legal grapple point, commence grapple
		{
			grapplePoint = pointOfContact;
			Rigidbody* rb = dynamic_cast<Rigidbody*>(po);
			//									strength, damping, restLength
			if(rb && !rb->IsKinematic())
				m_grapple = new Spring(m_player, rb, 6.f, 0.f, 0.1f, glm::vec2(0), glm::vec2(0));
			else
				m_grapple = new Spring(m_player, nullptr, 6.f, 0.f, 0.1f, glm::vec2(0), grapplePoint);

			m_physicsScene->AddActor(m_grapple);

			isGrappling = true;
		}
	}
	// Try to release grapple
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

void GamePlayScene::GenerateLevel()
{
	for (PhysicsObject* po : m_levelObjects)
		m_physicsScene->RemoveActor(po);

	for (PhysicsObject* lo : m_levelObjects)
		delete lo;

	m_levelObjects.clear();

	m_level++;
	if (m_level > 4)
		m_level = 1;
	
	std::vector<std::string> sLevel;

	sLevel = GetLevel(m_level);

	int* level = new int[400];

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			level[i * sLevel[0].length() + j] = sLevel[i][j] - '0';
		}
	}

	glm::vec2 blockSize = glm::vec2(9, 5);

	for (int i = 0; i < 400; i++)
	{
		if (level[i] == 1)
		{
			Box* box = new Box(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)),
				glm::vec2(0), 5.f, blockSize * 0.5f, 0.f, glm::vec4(0, 1, 0, 1));
			box->SetKinematic(true);
			m_levelObjects.push_back(box);

			box->SetAngularDrag(8.f); box->SetLinearDrag(0.3f); box->SetElasticity(0.1f);
		}
		if (level[i] == 2)
		{
			Box* box = new Box(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)),
				glm::vec2(0), 5.f, glm::vec2(blockSize.y * 0.4f), 0.f, glm::vec4(1, 1, 0, 1));
			box->SetKinematic(false);
			m_levelObjects.push_back(box);

			box->SetAngularDrag(16.f); box->SetLinearDrag(0.3f); box->SetElasticity(0.3f);
		}
		if (level[i] == 3)
		{
			m_targetBody->ResetPosition();
			m_targetBody->SetPosition(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)));
		}
		if(level[i] == 4)
		{
			m_player->ResetPosition();
			m_player->SetPosition(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)));
		}
	}

	delete[] level;

	for (PhysicsObject* po : m_levelObjects)
		m_physicsScene->AddActor(po);
}

std::vector<std::string> GamePlayScene::GetLevel(int level)
{
	std::vector<std::string> sLevel;

	switch (level)
	{
	case 1:
		sLevel.push_back("11111111111111111111");
		sLevel.push_back("10001000400000200001");
		sLevel.push_back("10011100000000200001");
		sLevel.push_back("10011100000000200001");
		sLevel.push_back("10011100000000000001");
		sLevel.push_back("10022200000000000001");
		sLevel.push_back("10002000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000100000000000001");
		sLevel.push_back("10000100000011110001");
		sLevel.push_back("10000100000013010001");
		sLevel.push_back("10000000000020010001");
		sLevel.push_back("10000000000020010001");
		sLevel.push_back("10010001000011110001");
		sLevel.push_back("10010001000000000001");
		sLevel.push_back("10010001000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111111111");
		break;

	case 2:
		sLevel.push_back("11111111111111111111");
		sLevel.push_back("10400000000000200001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111100001");
		sLevel.push_back("10000000000020200001");
		sLevel.push_back("10000110000020000001");
		sLevel.push_back("10000110000001111111");
		sLevel.push_back("10000110000000000001");
		sLevel.push_back("10000110000000000001");
		sLevel.push_back("10000110000000000001");
		sLevel.push_back("10000110000000000001");
		sLevel.push_back("10000220000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000011110001");
		sLevel.push_back("10000000000023020001");
		sLevel.push_back("10000000000020020001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111111111");
		break;

	case 3:
		sLevel.push_back("11111111111111111111");
		sLevel.push_back("10000000040000000001");
		sLevel.push_back("10010001000010001001");
		sLevel.push_back("10020002000020002001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000222200000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000100001000100001");
		sLevel.push_back("10000200002000200001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10010001000100010001");
		sLevel.push_back("10020002000200020001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000100000001000001");
		sLevel.push_back("10000200000002000001");
		sLevel.push_back("10000000010000000001");
		sLevel.push_back("10000000030000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111111111");
		break;

	case 4:
		sLevel.push_back("11111111111111111111");
		sLevel.push_back("10000000004000000001");
		sLevel.push_back("10202020202020202001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10202020202020202001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10202020202020202001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10202020202020202001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10202020201020202001");
		sLevel.push_back("10000000003000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111111111");
		break;

	default:
		sLevel.push_back("11111111111111111111");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("10000000000000000001");
		sLevel.push_back("11111111111111111111");
		break;

	}
	return sLevel;
}
