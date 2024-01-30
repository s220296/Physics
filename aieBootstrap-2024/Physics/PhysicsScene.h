#pragma once

#include "glm/vec2.hpp"
#include "vector"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void SetCurrentInstance(PhysicsScene* currentActiveInstance);

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void Update(float dt);
	void Draw();

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	//glm::vec2 GetGravity() const { return m_gravity; }
	static glm::vec2 GetGravity();

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	float GetTotalEnergy();

	static bool Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};