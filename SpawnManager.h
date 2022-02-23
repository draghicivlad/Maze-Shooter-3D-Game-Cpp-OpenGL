#pragma once
#include "components/simple_scene.h"
#include "lab_m1/Tema_2/MyObjectT2.h"

using namespace std;
#include <vector>

namespace m1
{
    class SpawnManager
    {
    public:
        pair<vector<shared_ptr<MyObjectT2>>, vector<glm::vec3>> GenerateMaze(vector<Mesh*> boxMesh, vector<vector<int>>
            mazeMatrix, glm::vec3 position, glm::vec3 scale, Shader* shader);

        shared_ptr<MyObjectT2> GeneratePlayer(vector<Mesh*> boxMesh, Shader* shader, glm::vec3 loc);
        shared_ptr<MyObjectT2> SpawnProj(Mesh* mMesh, glm::vec3 aPos, glm::vec3 aDir, float aSpeed, glm::vec3 aScale, Shader* mShader);
    };
}
