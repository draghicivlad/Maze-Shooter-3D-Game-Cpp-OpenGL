#include "lab_m1/Tema_2/transform3D.h"
#include "lab_m1/Tema_2/SpawnManager.h"
#include <vector>
#include <iostream>
#include <time.h>

#define HAS_POW(X, Y)  ((X & Y) != 0 ? (true) : (false))

using namespace std;
using namespace m1;

pair<vector<shared_ptr<MyObjectT2>>, vector<glm::vec3>> SpawnManager::GenerateMaze(vector<Mesh*> boxMesh,
	vector<vector<int>>mazeMatrix, glm::vec3 position, glm::vec3 scale, Shader *shader)
{
	vector<shared_ptr<MyObjectT2>> ans;
	vector<glm::vec3> validLoc;

	int n = mazeMatrix.size();

	int stanga = 1;
	int sus = 2;
	int dreapta = 4;
	int jos = 8;

	int start = - n / 2;
	int stop = 0;
	float offset = 0;
	float squareSize = scale.x / n;

	if (n % 2 == 0)
	{
		offset = squareSize / 2;
		stop = n / 2 - 1;
	}
	else
	{
		offset = 0;
		stop = n / 2;
	}

	int posI = 0;
	for (float i = start; i <= stop; i++)
	{
		int posJ = 0;
		for (float j = start; j <= stop; j++)
		{
			float centerX = j * squareSize + offset;
			float centerY = i * squareSize + offset;

			validLoc.push_back(glm::vec3(centerX, 0, centerY));

			if ((posI == 0) && (posJ == 0)) {
				posJ++;
				continue;
			}

			int cellType = mazeMatrix[posI][posJ];

			float wallT = 0.1f;
			float wallH = 2;

			if (!HAS_POW(cellType, stanga))
			{
				int index = rand() % boxMesh.size();
				ans.push_back(make_shared<MyObjectT2>(boxMesh[index], Transform(glm::vec3(centerX
					- squareSize / 2, position.y, centerY), glm::vec3(wallT, wallH, squareSize / 2)), Collision(glm::vec3(centerX -
						squareSize / 2 - wallT, position.y - wallH, centerY - squareSize / 2), glm::vec3(centerX - squareSize / 2 + wallT,
							position.y + wallH, centerY + squareSize / 2), true), shader));
			}

			if (!HAS_POW(cellType, dreapta))
			{
				int index = rand() % boxMesh.size();
				ans.push_back(make_shared<MyObjectT2>(boxMesh[index], Transform(glm::vec3(centerX
					+ squareSize / 2, position.y, centerY), glm::vec3(wallT, wallH, squareSize / 2)), Collision(glm::vec3(centerX
						+ squareSize / 2 - wallT, position.y - wallH, centerY - squareSize / 2), glm::vec3(centerX
							+ squareSize / 2 + wallT, position.y + wallH, centerY + squareSize / 2), true), shader));
			}

			if (!HAS_POW(cellType, sus))
			{
				int index = rand() % boxMesh.size();
				ans.push_back(make_shared<MyObjectT2>(boxMesh[index], Transform(glm::vec3(centerX,
					position.y, centerY - squareSize / 2), glm::vec3(squareSize / 2, wallH, wallT)), Collision(glm::vec3(centerX - squareSize / 2,
						position.y - wallH, centerY - squareSize / 2 - wallT), glm::vec3(centerX + squareSize / 2,
							position.y + wallH, centerY - squareSize / 2 + wallT), true), shader));
			}

			if (!HAS_POW(cellType, jos))
			{
				int index = rand() % boxMesh.size();
				ans.push_back(make_shared<MyObjectT2>(boxMesh[index], Transform(glm::vec3(centerX,
					position.y, centerY + squareSize / 2), glm::vec3(squareSize / 2, wallH, wallT)), Collision(glm::vec3(centerX - squareSize / 2,
						position.y - wallH, centerY + squareSize / 2 - wallT), glm::vec3(centerX + squareSize / 2,
							position.y + wallH, centerY + squareSize / 2 + wallT), true), shader));
			}
				
			posJ++;
		}
		posI++;
	}

	return { ans, validLoc };
}

shared_ptr<MyObjectT2> SpawnManager::GeneratePlayer(vector<Mesh*> boxMesh, Shader* shader, glm::vec3 loc)
{
	vector<Mesh*> mesh{ boxMesh[0], boxMesh[1], boxMesh[2], boxMesh[2], boxMesh[3], boxMesh[3] };
	vector<glm::vec3> displacement{ {0, 0.8, 0}, {0, 0.2, 0}, {0.4, 0.2, 0}, {-0.4, 0.2, 0}, {0.15, 0.45 - 1, 0}, {-0.15, 0.45 - 1, 0} };
	vector<glm::vec3> scale{ {0.35, 0.35, 0.35}, {0.5, 0.8, 0.3}, {0.2, 0.6, 0.2}, {0.2, 0.6, 0.2}, {0.2, 0.65, 0.2}, {0.2, 0.65, 0.2} };
	vector<glm::vec3> rot{ glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0) };

	vector<Animation *> anim { nullptr,
		nullptr,
		new Animation(glm::vec3(0, 0.3, 0), glm::vec3(-RADIANS(45), 0, 0), glm::vec3(RADIANS(45), 0, 0), 1, glm::vec3(0, 0, 0), true),
		new Animation(glm::vec3(0, 0.3, 0), glm::vec3(-RADIANS(45), 0, 0), glm::vec3(RADIANS(45), 0, 0), 1, glm::vec3(0, 0, 0), false),
		new Animation(glm::vec3(0, 0.4, 0), glm::vec3(-RADIANS(45), 0, 0), glm::vec3(RADIANS(45), 0, 0), 1, glm::vec3(0, 0, 0), true),
		new Animation(glm::vec3(0, 0.4, 0), glm::vec3(-RADIANS(45), 0, 0), glm::vec3(RADIANS(45), 0, 0), 1, glm::vec3(0, 0, 0), false) };

	Transform myTransform(loc, glm::vec3(0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0), 2);

	return make_shared<MyObjectT2>(mesh, displacement, scale, rot, myTransform, Collision(glm::vec3(-0.4, 0, -0.2), glm::vec3(0.4, 2, 0.2), true), shader, anim);
}
shared_ptr<MyObjectT2> SpawnManager::SpawnProj(Mesh *mMesh, glm::vec3 aPos, glm::vec3 aDir, float aSpeed, glm::vec3 aScale, Shader * mShader)
{
	Transform myTransform(aPos, aSpeed * aDir, aScale, glm::vec3(0), 0);

	return make_shared<MyObjectT2>(mMesh, myTransform, Collision(aPos - aScale * 0.7f - 1.0f, aPos + aScale * 0.7f - 1.0f, true), mShader);
}
