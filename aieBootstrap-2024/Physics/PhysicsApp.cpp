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
#include "Box.h"

#include <iostream>

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
	m_physicsScene->SetCurrentInstance(m_physicsScene);
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

	//aie::Gizmos::add2DCircle(glm::vec2(0), 3.f, 15, glm::vec4(1));

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

Circle* circleStore1;

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

	m_physicsScene->SetGravity(glm::vec2(0, -16));

	Circle* ball1 = new Circle(glm::vec2(20, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 20), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(30, 20), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);

	ball1->ApplyForce(glm::vec2(-50, 0));
	ball2->ApplyForce(glm::vec2(25, 40));
	ball3->ApplyForce(glm::vec2(25, 0));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -10.f, glm::vec4(1,1,0,1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), -40.f, glm::vec4(1,1,0,1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -40.f, glm::vec4(1,1,0,1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

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
	using glm::vec2;
	m_physicsScene->SetGravity(vec2(0));
	m_physicsScene->SetTimeStep(0.01f);

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(11.11, 0), 0.170f, 4, glm::vec4(1, 1, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(20, 0), glm::vec2(0, 0), 0.160f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(0, 0));
	ball2->ApplyForce(glm::vec2(0, 0));


#endif // BilliardSim

#ifdef NewtonsCradle
	using glm::vec2;
	m_physicsScene->SetGravity(vec2(0));
	m_physicsScene->SetTimeStep(0.01f);

	Circle* ball1 = new Circle(glm::vec2(-16, 0), glm::vec2(20, 0), 1.f, 4, glm::vec4(1, 1, 1, 1));
	Circle* ball2 = new Circle(glm::vec2(16, 0), glm::vec2(0, 0), 1.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 1.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball4 = new Circle(glm::vec2(8, 0), glm::vec2(0, 0), 1.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball5 = new Circle(glm::vec2(-8, 0), glm::vec2(0, 0), 1.f, 4, glm::vec4(1, 0, 0, 1));

	Plane* plane1 = new Plane(vec2(1, 0), -28, glm::vec4(0, 1, 0, 1));
	Plane* plane2 = new Plane(vec2(-1, 0), -28, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);


#endif // NewtonsCradle
	
#ifdef PotentialEnergyTest
	using glm::vec2;
	using glm::vec4;

	m_physicsScene->SetGravity(vec2(0,- 9.f));
	m_physicsScene->SetTimeStep(0.01f);

	Circle* ball1 = new Circle(vec2(0, 20), vec2(0, 0), 6.f, 4.f, vec4(0, 0, 1, 1));
	Plane* plane1 = new Plane(vec2(0, 1), 0, vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(plane1);
#endif // PotentialEnergyTest

#ifdef PlaneBoxTest
	using glm::vec2;

	m_physicsScene->SetGravity(vec2(0, -9.f));
	m_physicsScene->SetTimeStep(0.01f);

	Box* box1 = new Box(vec2(10, 20), vec2(0, 5), 5.f, vec2(2.f, 2.f), 0.f, glm::vec4(1, 1, 0, 1));
	Box* box2 = new Box(vec2(11, 10), vec2(10, 0), 5.f, vec2(2.f, 2.f), 0.f, glm::vec4(1, 1, 0, 1));
	Box* box3 = new Box(vec2(10, 30), vec2(0, 20), 5.f, vec2(2.f, 2.f), 0.f, glm::vec4(1, 1, 0, 1));
	Plane* plane1 = new Plane(vec2(0, 1), 0, glm::vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(vec2(0.3, -0.7), -40, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(vec2(-1, 0), -20, glm::vec4(1, 1, 1, 1));
	Circle* ball1 = new Circle(vec2(0, 20), vec2(0, -10), 6.f, 4.f, glm::vec4(0, 0, 1, 1));
	Circle* ball2 = new Circle(vec2(-20, 20), vec2(-10, 0), 3.f, 3.f, glm::vec4(0, 0, 1, 1));
	
	ball1->SetElasticity(0.3f);
	ball2->SetElasticity(0.5f);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

#endif // PlaneBoxTest

#ifdef BilliardsTest1
	using glm::vec2;
	using glm::vec4;

	m_physicsScene->SetGravity(glm::vec2(0.f));
	m_physicsScene->SetTimeStep(0.01f);

	Plane* plane1 = new Plane(vec2(1, 0), -80, vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(vec2(-1, -0), -80, vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(vec2(0, 1), -40, vec4(1, 1, 1, 1));
	Plane* plane4 = new Plane(vec2(0, -1), -40, vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	Circle* ball1 = new Circle(vec2(-40, 0), vec2(0), 3.f, 3.f, vec4(1));
	Circle* ball2 = new Circle(vec2(20, 0), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));
	Circle* ball3 = new Circle(vec2(27, 5), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));
	Circle* ball4 = new Circle(vec2(27, -5), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));
	Circle* ball5 = new Circle(vec2(34, 10), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));
	Circle* ball6 = new Circle(vec2(34, -10), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));
	Circle* ball7 = new Circle(vec2(34, 0), vec2(0), 3.f, 3.f, vec4(1, 0, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(ball6);
	m_physicsScene->AddActor(ball7);

	circleStore1 = ball1;
#endif // BilliardsTest1


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
#ifdef TotalEnergyPrint
	std::cout << m_physicsScene->GetTotalEnergy() << std::endl;
#endif // TotalEnergyPrint

#ifdef BilliardsTest1
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
		circleStore1->ApplyForce(glm::vec2(20, 0), glm::vec2(0));
#endif // BilliardsTest1


}

