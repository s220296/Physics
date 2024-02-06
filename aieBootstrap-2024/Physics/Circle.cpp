#include "Circle.h"
#include "Gizmos.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	: Rigidbody(ShapeType::CIRCLE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = colour;

	m_moment = 0.5f * mass * (radius * radius);
}

void Circle::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 15, m_color);

	aie::Gizmos::add2DLine(m_smoothedPosition,
		m_smoothedPosition + m_smoothedLocalX * m_radius,
		glm::vec4(1, 1, 1, 1));
}

bool Circle::IsInside(glm::vec2 point)
{
	return glm::distance(point, m_position) <= m_radius;
}