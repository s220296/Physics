#include "Circle.h"
#include "Gizmos.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	: Rigidbody(ShapeType::CIRCLE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = colour;
}

void Circle::Draw(float alpha)
{
	m_color.a = alpha;
	aie::Gizmos::add2DCircle(m_position, m_radius, 15, m_color);
}
