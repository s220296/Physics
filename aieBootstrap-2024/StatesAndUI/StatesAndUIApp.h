#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "glm/vec2.hpp"

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

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

public:
	static glm::vec2 worldMousePos;

protected:
	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	GameStateManager*	m_gameStateManager;
};