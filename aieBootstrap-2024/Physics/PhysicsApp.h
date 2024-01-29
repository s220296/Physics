#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"

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
};