#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "string"
#include "PhysicsScene.h"
#include "Demos.h"
#include "Circle.h"
#include "Plane.h"

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("./font/consolas_bold.ttf", 32);
	m_texture = new aie::Texture("./textures/ship.png");

	// Implement PHysics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);

	DemoStartUp(1);

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

#ifndef ProjectileTest
	aie::Gizmos::clear();

#endif // !ProjectileTest


	// impememt physics scene
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	DemoUpdate(input, deltaTime);
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	
	m_2dRenderer->setUVRect(0, 0, 1, 1);
	m_2dRenderer->drawSprite(m_texture, 200, 200, 100, 100);

	aie::Gizmos::add2DCircle(glm::vec2(0), 3.f, 15, glm::vec4(1));

	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, 
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// my fps counter
	//m_2dRenderer->drawText(m_font, std::to_string(getFPS()).c_str(), 640.f, 360.f);

	// jesse's fps counter
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0.f, getWindowHeight() - 32);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball);
#endif // NewtonsFirstLaw

#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball);
#endif // NewtonsSecondLaw

#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-20, 1), glm::vec2(0, 0), 8.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(70, 0));
	ball2->ApplyForce(glm::vec2(0, 0));

#endif // NewtonsThirdLaw

#ifdef PlaneBallTest

	m_physicsScene->SetGravity(glm::vec2(0, -6));

	Circle* ball1 = new Circle(glm::vec2(20, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 20), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(-30, 0));
	ball2->ApplyForce(glm::vec2(5, 20));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -10.f, glm::vec4(1,1,0,1));

	m_physicsScene->AddActor(plane1);

#endif // PlaneBallTest


#ifdef ProjectileTest

	SetupContinuousDemo(glm::vec2(-40, 0), 45, 40, -10);
	
	m_physicsScene->SetGravity(glm::vec2(0, -10));
	m_physicsScene->SetTimeStep(0.001f);

	float radius = 1.0f; float speed = 40.f;
	glm::vec2 startPos(-40, 0);
	float inclination = glm::pi<float>() / 4.0f;

	glm::vec2 velocity(glm::cos((inclination)), glm::sin((inclination)));
	velocity *= speed;

	m_physicsScene->AddActor(new Circle(startPos, velocity, 1, radius, glm::vec4(0, 1, 1, 1)));

#endif // ProjectileTest

#ifdef BilliardSim

#endif // BilliardSim


}

void PhysicsApp::SetupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 color = glm::vec4(1, 1, 0, 1);

	float xVel = glm::cos(glm::radians(inclination)) * speed;
	float yVel = glm::sin(glm::radians(inclination)) * speed;

	while (t <= 5)
	{
		// calculate the x, y position of the projectile at time t

		float x = startPos.x + xVel * t;
		float y = startPos.y + yVel * t + gravity * t * t * 0.5f;

		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, segments, color);
		t += tStep;
	}
}

void PhysicsApp::DemoUpdate(aie::Input* input, float dt)
{
	
}

