#pragma once

#include "PhysicsObject.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

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

protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    glm::vec4 m_color;
};

