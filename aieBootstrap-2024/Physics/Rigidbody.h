#pragma once

#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Rigidbody : public PhysicsObject {
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position,
        glm::vec2 velocity, float orientation, float mass);
    ~Rigidbody();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    void ApplyForce(glm::vec2 force);
    void ApplyForceToActor(Rigidbody* inputActor, glm::vec2 force);

    // So we can always get the values of our protected variables without tampering.
    glm::vec2 GetPosition() { return m_position; }
    float GetOrientatation() { return m_orientation; }
    glm::vec2 GetVelocity() { return m_velocity; }
    float GetMass() { return m_mass; }

    void ResolveCollision(Rigidbody* actor2);
    float GetKineticEnergy() { return m_mass * glm::length(m_velocity) * glm::length(m_velocity) * 0.5f; }

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;    //2D so we only need a single float to represent our orientation
};
