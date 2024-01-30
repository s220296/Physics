#pragma once

#include "glm/vec2.hpp"

enum ShapeType {
    PLANE = 0,
    CIRCLE
    //,BOX
};

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType shapeID) { m_shapeID = shapeID; };

public:
    virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    virtual void Draw(float alpha) = 0;
    virtual void ResetPosition() {};
    virtual ShapeType GetShapeID() { return m_shapeID; };

protected:
    ShapeType m_shapeID;
};

