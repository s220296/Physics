#include "GamePlayScene.h"
#include "Gizmos.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Input.h"
#include "PhysicsScene.h"
#include "Circle.h"

using glm::vec2;
using glm::vec4;

GamePlayScene::GamePlayScene() : GameScene("Gameplay")
{
}

void GamePlayScene::Enter()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetCurrentInstance(m_physicsScene);
	m_physicsScene->SetGravity(vec2(0, -9));
	m_physicsScene->SetTimeStep(0.01f);

	m_physicsScene->AddActor(new Circle(vec2(0), vec2(0), 5.f, 5.f, vec4(1)));
}

void GamePlayScene::Update(float dt)
{
	m_physicsScene->Update(dt);
}

void GamePlayScene::Draw()
{
	m_physicsScene->Draw();

	aie::Gizmos::add2DCircle(vec2(0), 5, 15, vec4(1, 1, 0, 1));
}
