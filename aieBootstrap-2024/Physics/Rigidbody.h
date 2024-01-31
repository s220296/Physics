#pragma once

#include "PhysicsObject.h"
#include "glm/glm.hpp"

class Rigidbody : public PhysicsObject {
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position,
        glm::vec2 velocity, float orientation, float mass);
    ~Rigidbody();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    void ApplyForce(glm::vec2 force, glm::vec2 pos);

    void CalculateAxes();

    // So we can always get the values of our protected variables without tampering.
    glm::vec2 GetPosition() { return m_position; }
    float GetOrientatation() { return m_orientation; }
    glm::vec2 GetVelocity() { return m_velocity; }
    float GetMass() { return m_mass; }
    float GetMoment() { return m_moment; }
    float GetAngularVelocity() { return m_angularVelocity; }

    void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, 
        glm::vec2* collisionNormal=nullptr);

    float GetKineticEnergy();
    float GetGravitationalPotentialEnergy();
    float GetEnergy() override { return GetKineticEnergy() + GetGravitationalPotentialEnergy(); }

    void CalculateSmoothedPosition(float alpha);

    glm::vec2 GetLocalX() { return m_localX; }
    glm::vec2 GetLocalY() { return m_localY; }

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    
    float m_orientation;    //2D so we only need a single float to represent our orientation
    float m_lastOrientation;

    float m_angularVelocity;
    float m_moment;

    glm::vec2 m_lastPosition;
    glm::vec2 m_smoothedPosition;
    glm::vec2 m_smoothedLocalX;
    glm::vec2 m_smoothedLocalY;

    //store the local x,y axes of the box based on its angle of rotation
    glm::vec2 m_localX;
    glm::vec2 m_localY;
};
