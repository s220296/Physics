#include "Rigidbody.h"
#include "PhysicsScene.h"
#include "iostream"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, 
	glm::vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;	
	m_lastPosition = position;

	m_velocity = velocity;

	m_orientation = orientation; 
	m_lastOrientation = orientation;

	m_mass = mass;
	m_angularVelocity = 0.f;
	m_moment = 0.f;

	m_localX = glm::vec2(0);
	m_localY = glm::vec2(0);
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	CalculateAxes();

	m_lastPosition = m_position;
	
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));

	m_lastOrientation = m_orientation;
	m_orientation += m_angularVelocity * timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}

void Rigidbody::CalculateAxes()
{
	float sn = sinf(m_orientation);
	float cs = cosf(m_orientation);

	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact,
	glm::vec2* collisionNormal)
{
	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->m_position - m_position);
	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, 
// for both linear and rotational 		

		// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) +
		r2 * actor2->m_angularVelocity;
	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		//apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + 
		m_moment * m_angularVelocity * m_angularVelocity);
}

float Rigidbody::GetGravitationalPotentialEnergy()
{
	return -m_mass * glm::dot(PhysicsScene::GetGravity(), m_position);
}

void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation
		+ (1 - alpha) * m_lastOrientation;

	float sn = sinf(smoothedOrientation);
	float cs = cosf(smoothedOrientation);

	m_smoothedLocalX = glm::vec2(cs, sn);
	m_smoothedLocalY = glm::vec2(-sn, cs);
}
