#pragma once

#include "glm/vec2.hpp"

enum ShapeType {
    PLANE = 0
    , CIRCLE
    , BOX
};

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType shapeID) { m_shapeID = shapeID; m_elasticity = 1.f; }

public:
    virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void Draw(float alpha) = 0;
    virtual void ResetPosition() {}
    virtual ShapeType GetShapeID() { return m_shapeID; }
    virtual float GetEnergy() { return 0; }
    virtual float GetElasticity() { return m_elasticity; }

protected:
    ShapeType m_shapeID;

    float m_elasticity;
};

