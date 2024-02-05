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

	m_linearDrag = 0.3f;
	m_angularDrag = 0.3f;

	m_isKinematic = false;

	CalculateAxes();
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0.f;

		return;
	}

	CalculateAxes();

	m_lastPosition = m_position;
	
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));

	m_lastOrientation = m_orientation;
	m_orientation += m_angularVelocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (glm::abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
		m_angularVelocity = 0.f;
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

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact,
	glm::vec2* collisionNormal, float pen)
{
	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->GetPosition() - GetPosition());
	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	glm::vec2 relativeVelocity = actor2->GetVelocity() - GetVelocity();

	// determine the total velocity of the contact points for the two objects, 
	// for both linear and rotational 

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - GetPosition(), -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(GetVelocity(), normal) - r1 * GetAngularVelocity();
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->GetVelocity(), normal) +
		r2 * actor2->GetAngularVelocity();

	if (v1 > v2) // they're moving closer
	{
		if (pen > 0) // Seperate by overlap amount
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);

		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / GetMass()) + (1 / actor2->GetMass())));

		glm::vec2 force = j * normal;

		//apply equal and opposite forces
		ApplyForce(-force, contact - GetPosition());
		actor2->ApplyForce(force, contact - actor2->GetPosition());
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
