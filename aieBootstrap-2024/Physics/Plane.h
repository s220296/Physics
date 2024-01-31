#pragma once

#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Rigidbody;

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 normal, float distance, glm::vec4 color);
    ~Plane();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    virtual void Draw(float alpha);
    virtual void ResetPosition();

    glm::vec2 GetNormal() { return m_normal; }
    float GetDistance() { return m_distanceToOrigin; }
    glm::vec4 GetColor() { return m_color; }

    void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);

protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    glm::vec4 m_color;
};

