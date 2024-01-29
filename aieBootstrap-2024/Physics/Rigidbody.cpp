#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, 
	glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;	
	m_velocity = velocity;
	m_orientation = orientation; 
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	if(m_mass != 0.f)
		m_velocity += force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* inputActor, glm::vec2 force)
{
	inputActor->ApplyForce(force);
	ApplyForce(-force);
}
