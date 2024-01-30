#include "Plane.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include "Rigidbody.h"
#include "iostream"

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 color) : PhysicsObject(PLANE)
{
	m_normal = glm::normalize(normal);
	m_distanceToOrigin = distance;
    m_color = color;
}

Plane::~Plane()
{
}

void Plane::ResolveCollision(Rigidbody* actor2)
{
	glm::vec2 normal = glm::normalize(m_normal);
	glm::vec2 relativeVelocity = actor2->GetVelocity();

	// if the objects are already moving apart, we don't need to do anything
	if (glm::dot(normal, relativeVelocity) >= 0)
		return;

	float elasticity = 1.f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / ((1 / actor2->GetMass()));

	glm::vec2 force = normal * j;

	float kePre = actor2->GetKineticEnergy();

	actor2->ApplyForce(force); // -force changed to force

	float kePost = actor2->GetKineticEnergy();

	float deltaKE = kePost - kePre;
	if (glm::abs(deltaKE) > kePost * 0.01f)
		std::cout << "Kinetic Energy discrepancy greater than 1% detected";
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw(float alpha)
{
    float lineSegmentLength = 300;
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
    // easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);
    glm::vec4 colourFade = m_color;
    colourFade.a = 0;
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    //aie::Gizmos::add2DLine(start, end, colour);
    aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}

void Plane::ResetPosition()
{
	m_distanceToOrigin = 0.f;
}
