#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include "glm/vec2.hpp"

class PhysicsScene;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	aie::Texture*		m_texture;
	PhysicsScene*		m_physicsScene;

	float				m_cameraX;
	float				m_cameraY;

protected:
	void UserUpdate(float dt);
	void GrabAndMove(aie::Input* input);

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

	// ===== For Demos Only =====
public:
	void DemoStartUp(int num);
	void DemoUpdate(aie::Input* input, float dt);
	void SetupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);
};