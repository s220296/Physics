#include "StatesAndUIApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/ext.hpp"
#include "GameStateManager.h"
#include "GamePlayScene.h"
#include "MenuScene.h"
#include "PhysicsScene.h"
#include "Circle.h"

using glm::vec2;
using glm::vec4;

glm::vec2 StatesAndUIApp::worldMousePos;

StatesAndUIApp::StatesAndUIApp() {

}

StatesAndUIApp::~StatesAndUIApp() {

}

bool StatesAndUIApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_gameStateManager = new GameStateManager(new GamePlayScene());
	m_gameStateManager->AddGameState(new MenuScene());

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	return true;
}

void StatesAndUIApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;

	delete m_gameStateManager;
}

void StatesAndUIApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	m_gameStateManager->Update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	glm::vec2 mouse(input->getMouseX(), input->getMouseY());
	worldMousePos = ScreenToWorld(mouse);
}

void StatesAndUIApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	//aie::Gizmos::add2DCircle(vec2(0), 5.f, 15, vec4(1, 0, 0, 1));

	m_gameStateManager->Draw();

	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	aie::Gizmos::clear();

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 StatesAndUIApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0, 0)
	worldPos.x -= getWindowWidth() * 0.5f;
	worldPos.y -= getWindowHeight() * 0.5f;

	//scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}
