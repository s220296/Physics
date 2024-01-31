#include "Box.h"
#include "Gizmos.h"

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, 
	float orientation, glm::vec4 colour)
	: Rigidbody(ShapeType::BOX, position, velocity, orientation, mass)
{
	m_extents = extents;
	m_color = colour;
}

void Box::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	// draw using local axes
	glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;
	glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;
	glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;
	glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);

	// /\ doesnt work --- so im using \/

	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_color);
}
