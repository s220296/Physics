#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Circle.h"
#include "glm/glm.hpp"
#include "Plane.h"

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};

static const int SHAPE_COUNT = 2;

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

void PhysicsScene::Update(float dt) {

    // update physics at a fixed time step
    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while (accumulatedTime >= m_timeStep) {
        for (auto pActor : m_actors) {
            pActor->FixedUpdate(m_gravity, m_timeStep);
        }
        accumulatedTime -= m_timeStep;

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

                // using function pointers
                int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
                fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
                if (collisionFunctionPtr != nullptr)
                {
                    // did a collision occur?
                    collisionFunctionPtr(object1, object2);
                }
            }
        }
    }
}

void PhysicsScene::Draw()
{
    for (auto pActor : m_actors) {
        pActor->Draw(1);
    }
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
            plane->ResolveCollision(circle);
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
        // TODO do the necessary maths in here

        float minDistance = circle1->GetRadius() + circle2->GetRadius();

        // TODO if the Circles touch, set their velocities to zero for now

        if (glm::distance(circle1->GetPosition(), circle2->GetPosition()) < minDistance)
        {
            circle1->ResolveCollision(circle2);
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
	m_actors.erase(std::find(m_actors.begin(), m_actors.end(), actor));
}
