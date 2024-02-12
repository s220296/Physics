#include "Spring.h"
#include "glm/glm.hpp"
#include "Rigidbody.h"
#include "Gizmos.h"

Spring::Spring(Rigidbody* body1, Rigidbody* body2, float springCoefficient, 
	float damping, float restLength, glm::vec2 contact1, glm::vec2 contact2, glm::vec4 color)
	: PhysicsObject(JOINT)
{
	m_color = color;

	m_body1 = body1; m_body2 = body2;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = restLength;

	m_contact1 = contact1;
	m_contact2 = contact2;

	if (restLength == 0)
	{
		if (body1) body1->CalculateAxes();
		if (body2) body2->CalculateAxes();
		m_restLength = glm::distance(GetContact1(), GetContact2());
	}
}

void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	// Get the world coordinates of the ends of the springs
	glm::vec2 p1 = GetContact1();
	glm::vec2 p2 = GetContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	//apply damping
	glm::vec2 relativeVelocity = glm::vec2(0);
	if(m_body1 && m_body2)
	relativeVelocity = m_body2->GetVelocity() - m_body1->GetVelocity();

	// F = -kX - bv
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	const float threshold = 1000.0f;
	float forceMag = glm::length(force);
	if (forceMag > threshold)
		force *= threshold / forceMag;

	if(m_body1) m_body1->ApplyForce(-force * timeStep, p1 - m_body1->GetPosition());
	if(m_body2) m_body2->ApplyForce(force * timeStep, p2 - m_body2->GetPosition());		
}

void Spring::Draw(float alpha)
{
	aie::Gizmos::add2DLine(GetSmoothedContact1(alpha), GetSmoothedContact2(alpha), m_color);
}

glm::vec2 Spring::GetContact1(float alpha)
{
	return m_body1 ? m_body1->ToWorld(m_contact1, alpha) : m_contact1;
}

glm::vec2 Spring::GetContact2(float alpha)
{
	return m_body2 ? m_body2->ToWorld(m_contact2, alpha) : m_contact2;
}

bool Spring::IsInside(glm::vec2 point)
{
	return false;

	// TODO: this doesn't work, but its close

	const float tolerance = 0.1f;

	glm::vec2 closestPointHorizontally = glm::vec2(0);
	glm::vec2 closestPointVertically = glm::vec2(0);
	glm::vec2 contactNormal = glm::normalize(GetContact1() - GetContact2());
	glm::vec2 pointDiff = point - GetContact2();

	closestPointHorizontally = GetContact2() + (((pointDiff).x / contactNormal.x) * contactNormal);
	closestPointVertically = GetContact2() + (((pointDiff).y / contactNormal.y) * contactNormal);

	aie::Gizmos::add2DCircle(closestPointHorizontally, 2.f, 5, glm::vec4(1, 1, 0, 1));
	aie::Gizmos::add2DCircle(closestPointVertically, 2.f, 5, glm::vec4(1, 0, 1, 1));

	if (glm::distance(closestPointHorizontally, point) <= tolerance &&
		glm::distance(closestPointVertically, point) <= tolerance)
		return true;

	return false;
}

glm::vec2 Spring::GetSmoothedContact1(float alpha)
{
	return m_body1 ? m_body1->ToWorldSmoothed(m_contact1, alpha) : m_contact1;
}

glm::vec2 Spring::GetSmoothedContact2(float alpha)
{
	return m_body2 ? m_body2->ToWorldSmoothed(m_contact2, alpha) : m_contact2;
}
