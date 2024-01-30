#include "MenuScene.h"
#include "EasyDraw.h"

MenuScene::MenuScene() : GameScene("Menu")
{
}

void MenuScene::Update(float dt)
{
}

void MenuScene::Draw()
{
	aie::Gizmos::add2DCircle(vec2(0), 5, 15, vec4(0, 1, 1, 1));
}
