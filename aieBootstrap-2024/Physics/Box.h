#pragma once

#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity,
		float mass, glm::vec2 extents, float orientation, glm::vec4 colour);
	~Box();

	virtual void Draw(float alpha);

	glm::vec2 GetExtents() { return m_extents; }
	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }

protected:
	glm::vec4 m_color;

	glm::vec2 m_extents;		//the halfedge extents
};

