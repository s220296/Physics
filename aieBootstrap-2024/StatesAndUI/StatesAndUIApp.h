#pragma once

#include "Application.h"
#include "Renderer2D.h"

class GameStateManager;
class PhysicsScene;

class StatesAndUIApp : public aie::Application {
public:

	StatesAndUIApp();
	virtual ~StatesAndUIApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	GameStateManager*	m_gameStateManager;
};