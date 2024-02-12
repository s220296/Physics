#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Circle.h"
#include "glm/glm.hpp"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Circle,  PhysicsScene::Plane2Box,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
    PhysicsScene::Box2Plane,    PhysicsScene::Box2Circle,    PhysicsScene::Box2Box
};

static PhysicsScene* m_instance;

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
    {
        delete pActor;
    }
}

void PhysicsScene::SetCurrentInstance(PhysicsScene* currentActiveInstance)
{
    m_instance = currentActiveInstance;
}

void PhysicsScene::Update(float dt) {

    // update physics at a fixed time step
    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while (accumulatedTime >= m_timeStep) {
        for (auto pActor : m_actors) {
            pActor->FixedUpdate(m_gravity, m_timeStep);
        }
        accumulatedTime -= m_timeStep;

        CheckForCollision();
    }
}

void PhysicsScene::Draw()
{
    for (auto pActor : m_actors) {
        pActor->Draw(1);
    }
}

void PhysicsScene::ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen)
{
    if ((body1 && body1->IsTrigger()) || (body2 && body2->IsTrigger()))
        return;

    float body2Mass = body2 ? body2->GetMass() : INT_MAX;
    float body1Factor = body2Mass / (body1->GetMass() + body2Mass);

    body1->SetPosition(body1->GetPosition() - body1Factor * norm * pen);

    if (body2)
        body2->SetPosition(body2->GetPosition() + (1 - body1Factor) * norm * pen);
}

glm::vec2 PhysicsScene::GetGravity()
{
    return m_instance->m_gravity;
}

float PhysicsScene::GetTotalEnergy()
{
    float total = 0;
    for (auto it = m_actors.begin(); it != m_actors.end(); it++)
    {
        PhysicsObject* obj = *it;
        total += obj->GetEnergy();
    }
    return total;
}

void PhysicsScene::CheckForCollision()
{
    // check for collisions (ideally you'd want to have some sort of 
        // scene management in place)
    int actorCount = m_actors.size();

    // need to check for collisions against all objects except this one.
    for (int outer = 0; outer < actorCount - 1; outer++)
    {
        for (int inner = outer + 1; inner < actorCount; inner++)
        {
            PhysicsObject* object1 = m_actors[outer];
            PhysicsObject* object2 = m_actors[inner];
            int shapeId1 = object1->GetShapeID();
            int shapeId2 = object2->GetShapeID();

            // this check will ensure we don't include any joints
            // in the collision checks
            if (shapeId1 < 0 || shapeId2 < 0)
                continue;

            // using function pointers
            int functionIdx = (shapeId1 * SHAPECOUNT) + shapeId2;
            fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
            if (collisionFunctionPtr != nullptr)
            {
                // did a collision occur?
                collisionFunctionPtr(object1, object2);
            }
        }
    }
}

PhysicsObject* PhysicsScene::PointCast(glm::vec2 point)
{
    for (PhysicsObject* po : m_actors)
    {
        if (po->IsInside(point))
        {
            return po;
        }
    }

    return nullptr;
}

bool PhysicsScene::Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return Circle2Plane(obj2, obj1);
}

bool PhysicsScene::Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    Circle* circle = dynamic_cast<Circle*>(obj1);
    Plane* plane = dynamic_cast<Plane*>(obj2);
    //if we are successful then test for collision
    if (circle != nullptr && plane != nullptr)
    {
        glm::vec2 collisionNormal = plane->GetNormal();
        float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

        float intersection = circle->GetRadius() - sphereToPlane;
        float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
        if (intersection > 0 && velocityOutOfPlane < 0)
        {
            glm::vec2 contact = circle->GetPosition() + 
                (collisionNormal * -circle->GetRadius());

            plane->ResolveCollision(circle, contact);
            return true;
        }
    }
    return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
    // try to cast objects to Circle and Circle
    Circle* circle1 = dynamic_cast<Circle*>(obj1);
    Circle* circle2 = dynamic_cast<Circle*>(obj2);
    // if we are successful then test for collision
    if (circle1 != nullptr && circle2 != nullptr)
    {
        float dist = glm::distance(circle1->GetPosition(), circle2->GetPosition());

        float penetration = circle1->GetRadius() + circle2->GetRadius() - dist;

        if(penetration > 0)
        {
            circle1->ResolveCollision(circle2, 0.5f * 
                (circle1->GetPosition() + circle2->GetPosition()), nullptr, penetration);

            return true;
        }
    }

    return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
    Box* box1 = dynamic_cast<Box*>(obj1);
    Box* box2 = dynamic_cast<Box*>(obj2);

    if (box1 != nullptr && box2 != nullptr)
    {
        glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
        glm::vec2 norm(0, 0);
        glm::vec2 contact(0, 0);
        float pen = 0;
        int numContacts = 0;
        
        box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
        if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
        {
            norm = -norm;
        }
        if (pen > 0)
        {
            box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
            return true;
        }

    }

    return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return Plane2Box(obj2, obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
    Plane* plane = dynamic_cast<Plane*>(obj1);
    Box* box = dynamic_cast<Box*>(obj2);

    //if we are successful then test for collision
    if (box != nullptr && plane != nullptr)
    {
        int numContacts = 0;
        glm::vec2 contact(0, 0);
        float contactV = 0;

        // Get a representative point on the plane

        // plane->GetNormal() is returning a weird number, fix this

        glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

        // check all four corners to see if we've hit the plane
        for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
        {
            for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
            {
                // Get the position of the corner in world space
                glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
                float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

                // this is the total velocity of the point in world space
                glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
                glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
                // and this is the component of the point velocity into the plane
                float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

                // and moving further in, we need to resolve the collision
                if (distFromPlane < 0 && velocityIntoPlane <= 0)
                {
                    numContacts++;
                    contact += p;
                    contactV += velocityIntoPlane;
                }
            }
        }

        // we've had a hit - typically only two corners can contact
        if (numContacts > 0)
        {
            plane->ResolveCollision(box, contact / (float)numContacts);
            return true;
        }
    }

    return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return Box2Circle(obj2, obj1);
}

bool PhysicsScene::Box2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
    Box* box = dynamic_cast<Box*>(obj1);
    Circle* circle = dynamic_cast<Circle*>(obj2);

    if (box != nullptr && circle != nullptr)
    {
        // transform the circle into the box's coordinate space
        glm::vec2 circlePosWorld = circle->GetPosition() - box->GetPosition();
        glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()),
            glm::dot(circlePosWorld, box->GetLocalY()));

        // find the closest point to the circle centre on the box by clamping the
        // coordinates in box-space to the box's extents

        glm::vec2 closestPointOnBoxBox = circlePosBox;
        glm::vec2 extents = box->GetExtents();

        if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
        if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
        if (closestPointOnBoxBox.y < -extents.x) closestPointOnBoxBox.y = -extents.y;
        if (closestPointOnBoxBox.y > extents.x) closestPointOnBoxBox.y = extents.y;

        // and convert back into world coordinates

        glm::vec2 closestPointOnBoxWorld = box->GetPosition() +
            closestPointOnBoxBox.x * box->GetLocalX() + 
            closestPointOnBoxBox.y * box->GetLocalY();
        glm::vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;
        float penetration = circle->GetRadius() - glm::length(circleToBox);

        if (penetration > 0)
        {
            glm::vec2 direction = glm::normalize(circleToBox);
            glm::vec2 contact = closestPointOnBoxWorld;
            box->ResolveCollision(circle, contact, &direction, penetration);

            return true;
        }
    }

    return false;
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
    auto it = std::find(m_actors.begin(), m_actors.end(), actor);
    if(it != m_actors.end())
	    m_actors.erase(it);
}
