#pragma once

#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity,
		float mass, glm::vec2 extents, float orientation, glm::vec4 colour);
	~Box();

	virtual void Draw(float alpha);

	glm::vec2 GetExtents() const { return m_extents; }
	float GetWidth() const { return m_extents.x * 2; }
	float GetHeight() const { return m_extents.y * 2; }

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts,
		float& pen, glm::vec2& edgeNormal);
		
protected:
	glm::vec4 m_color;

	glm::vec2 m_extents;		//the halfedge extents
};

