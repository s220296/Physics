#pragma once

#include "glm/vec2.hpp"
#include "vector"
#include "string"

class PhysicsScene;

class SoftBody
{
public:
	static void Build(PhysicsScene* scene, glm::vec2 position, float damping,
		float springForce, float spacing, std::vector<std::string>& strings);
};

