#pragma once

#include "PhysicsObject.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

class Rigidbody;

class Spring : public PhysicsObject
{
public:
	Spring(Rigidbody* body1, Rigidbody* body2, float springCoefficient, float damping,
		float restLength = 0.f, glm::vec2 contact1 = glm::vec2(0, 0),
		glm::vec2 contact2 = glm::vec2(0, 0), glm::vec4 color = glm::vec4(1,1,1,1));
	~Spring() {}

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) override;
	virtual void Draw(float alpha) override;

	glm::vec2 GetContact1(float alpha = 1.f);
	glm::vec2 GetContact2(float alpha = 1.f);

protected:
	glm::vec2 GetSmoothedContact1(float alpha = 1.f);
	glm::vec2 GetSmoothedContact2(float alpha = 1.f);

protected:
	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	glm::vec4 m_color;

	float m_damping;
	float m_restLength;
	float m_springCoefficient; // the restoring force
};

