#include "Player.h"
#include "Box.h"

#include <string>
#include <vector>
#include <iostream>

Player::Player(Circle* circleBody)
{
	body = circleBody;
	level = nullptr;
}

Player::~Player()
{
	delete body;
}

std::vector<PhysicsObject*> Player::GenerateLevel()
{
	std::vector<std::string> sLevel;

	// 20 x 20
	sLevel.push_back("11111111111111111111");
	sLevel.push_back("10001000000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10011100000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000011110001");
	sLevel.push_back("10000000000010010001");
	sLevel.push_back("10000000000010010001");
	sLevel.push_back("10000000000000010001");
	sLevel.push_back("10000000000011110001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("10000000000000000001");
	sLevel.push_back("11111111111111111111");

	level = new int[400];
	
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			level[i * sLevel[0].length() + j] = sLevel[i][j] - '0';
		}
	}

	std::vector<PhysicsObject*> result;
	glm::vec2 blockSize = glm::vec2(9, 5);

	for (int i = 0; i < 400; i++)
	{
		if (level[i] == 1)
		{
			Box* box = new Box(glm::vec2(blockSize.x * (i % 20) - (blockSize.x * 9), blockSize.y * (i / 20) - (blockSize.y * 9)),
				glm::vec2(0), 5.f, blockSize * 0.5f, 0.f, glm::vec4(0, 1, 0, 1));
			box->SetKinematic(true);
			result.push_back(box);
		}
	}

	return result;
}
