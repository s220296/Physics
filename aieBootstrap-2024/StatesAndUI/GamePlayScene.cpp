#include "GamePlayScene.h"
#include "Gizmos.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Input.h"

using glm::vec2;
using glm::vec4;

GamePlayScene::GamePlayScene() : GameScene("Gameplay")
{
}

void GamePlayScene::Update(float dt)
{
}

void GamePlayScene::Draw()
{
	aie::Gizmos::add2DCircle(vec2(0), 5, 15, vec4(1, 1, 0, 1));
}
