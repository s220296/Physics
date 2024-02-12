#pragma once

#include "glm/vec2.hpp"
#include "vector"

class PhysicsObject;
class Rigidbody;

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

	static void ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen);

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	//glm::vec2 GetGravity() const { return m_gravity; }
	static glm::vec2 GetGravity();

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	float GetTotalEnergy();

	void CheckForCollision();
	
	PhysicsObject* PointCast(glm::vec2 point);
	PhysicsObject* LineCast(PhysicsObject* ignore, glm::vec2 position, glm::vec2 direction, float distance, glm::vec2& pointOfContact);

	std::vector<PhysicsObject*> GetActors() { return m_actors; }

	static bool Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Circle2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Circle(PhysicsObject* obj1, PhysicsObject* obj2);
	
protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};