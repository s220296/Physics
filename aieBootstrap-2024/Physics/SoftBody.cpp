#include "SoftBody.h"
#include "Circle.h"
#include "PhysicsScene.h"
#include "Spring.h"

void SoftBody::Build(PhysicsScene* scene, glm::vec2 position, float damping, float springForce, float spacing, std::vector<std::string>& strings)
{
	int numColumns = strings.size();
	int numRows = strings[0].length();

	// traverse across the array and add balls where the ascii art says they should be
	Circle** circles = new Circle * [numRows * numColumns];

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (strings[j][i] == '0')
			{
				circles[i * numColumns + j] =
					new Circle(position + glm::vec2(i, j) * spacing, //pos
					glm::vec2(0, 0), 1.0f, 2.0f, glm::vec4(1, 0, 0, 1));//vel, mass, radius, color
				scene->AddActor(circles[i * numColumns + j]);
			}
			else
			{
				circles[i * numColumns + j] = nullptr;
			}
		}
	}

	// second pass - add springs in
	for (int i = 1; i < numRows; i++)
	{
		for (int j = 1; j < numColumns; j++)
		{
			// s00 s01
			// s10 s11
			Circle* s11 = circles[i * numColumns + j];
			Circle* s01 = circles[(i - 1) * numColumns + j];
			Circle* s10 = circles[i * numColumns + j - 1];
			Circle* s00 = circles[(i - 1) * numColumns + j - 1];

			// make springs to cardinal neighbours
			if (s11 && s01)
				scene->AddActor(new Spring(s11, s01, springForce, damping, spacing));
			if (s11 && s10)
				scene->AddActor(new Spring(s11, s10, springForce, damping, spacing));
			if (s10 && s00)
				scene->AddActor(new Spring(s10, s00, springForce, damping, spacing));
			if (s01 && s00)
				scene->AddActor(new Spring(s01, s00, springForce, damping, spacing));

			// Diags / Shears
			float diag = glm::sqrt(2);
			if (s11 && s00)
				scene->AddActor(new Spring(s11, s00, springForce, damping, spacing * diag));
			if (s01 && s10)
				scene->AddActor(new Spring(s01, s10, springForce, damping, spacing * diag));

			// Bends
			bool endOfJ = j == numColumns - 1;
			bool endOfI = i == numRows - 1;

			Circle* s22 = (!endOfI && !endOfJ) ? circles[(i + 1) * numColumns + (j + 1)] : nullptr;
			Circle* s02 = !endOfJ ? circles[(i - 1) * numColumns + (j + 1)] : nullptr;
			Circle* s20 = !endOfI ? circles[(i + 1) * numColumns + j - 1] : nullptr;

			if (s22 && s02)
				scene->AddActor(new Spring(s22, s02, springForce, damping, spacing * 2));
			if (s22 && s20)
				scene->AddActor(new Spring(s22, s20, springForce, damping, spacing * 2));
			if (s00 && s02)
				scene->AddActor(new Spring(s00, s02, springForce, damping, spacing * 2));
			if (s00 && s20)
				scene->AddActor(new Spring(s00, s20, springForce, damping, spacing * 2));
		}
	}
}
