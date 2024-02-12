#pragma once

#include "PhysicsObject.h"
#include "glm/glm.hpp"
#include <functional>
#include <list>

const static float MIN_LINEAR_THRESHOLD = 0.01f;
const static float MIN_ANGULAR_THRESHOLD = 0.3f;

class Rigidbody : public PhysicsObject {
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position,
        glm::vec2 velocity, float orientation, float mass);
    ~Rigidbody();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    void ApplyForce(glm::vec2 force, glm::vec2 pos);

    void CalculateAxes();

    // So we can always get the values of our protected variables without tampering.
    glm::vec2 GetPosition() const { return m_position; }
    float GetOrientatation() const { return m_orientation; }
    glm::vec2 GetVelocity() const { return m_velocity; }

    float GetMass() const { return m_isKinematic ? INT_MAX : m_mass; }
    float GetMoment() const { return m_isKinematic ? INT_MAX : m_moment; }
    float GetAngularVelocity() const { return m_angularVelocity; }

    void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, 
        glm::vec2* collisionNormal=nullptr, float pen = 0);

    float GetKineticEnergy();
    float GetGravitationalPotentialEnergy();
    float GetEnergy() override { return GetKineticEnergy() + GetGravitationalPotentialEnergy(); }

    void CalculateSmoothedPosition(float alpha);

    glm::vec2 GetLocalX() const { return m_localX; }
    glm::vec2 GetLocalY() const { return m_localY; }

    void SetLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
    void SetAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }

    void SetPosition(glm::vec2 position) { m_position = position; }

    void SetKinematic(bool state) { m_isKinematic = state; }
    bool IsKinematic() { return m_isKinematic; }

    void SetTrigger(bool state) { m_isTrigger = state; }
    bool IsTrigger() { return m_isTrigger; }

    glm::vec2 ToWorld(glm::vec2 contact, float alpha);
    glm::vec2 ToWorldSmoothed(glm::vec2 localPos, float alpha);

    void TriggerEnter(PhysicsObject* actor2);
    void TriggerExit(PhysicsObject* actor2);

public:
    std::function<void(PhysicsObject*)> collisionCallback;

    std::function<void(PhysicsObject*)> triggerEnter;
    std::function<void(PhysicsObject*)> triggerExit;

protected:
    bool m_isTrigger;
    std::list<PhysicsObject*> m_objectsInside;
    std::list<PhysicsObject*> m_objectsInsideThisFrame;

    bool m_isKinematic;

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

    float m_linearDrag; // 0 to 1
    float m_angularDrag; // 0 to 1
};
