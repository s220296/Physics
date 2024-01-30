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

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	aie::Texture*		m_texture;
	PhysicsScene*		m_physicsScene;

	// ===== For Demos Only =====
public:
	void DemoStartUp(int num);
	void DemoUpdate(aie::Input* input, float dt);
	void SetupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);
};