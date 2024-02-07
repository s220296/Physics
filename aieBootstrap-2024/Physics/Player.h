#pragma once
#include "glm/vec2.hpp"
#include <vector>

class Circle;
class PhysicsObject;

class Player
{
public:
	Player(Circle* circleBody);
	~Player();

	std::vector<PhysicsObject*> GenerateLevel();

public:
	Circle* body;
	int* level;
};

