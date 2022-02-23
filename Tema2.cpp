#include "lab_m1/Tema_2/Tema2.h"
#include "lab_m1/Tema_2/meshCreator.h"
#include "lab_m1/Tema_2/Collision.h"
#include "lab_m1/Tema_2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    
    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema_2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema_2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ExpShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema_2", "shaders", "ExpVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema_2", "shaders", "ExpFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        meshes["square"] = CreateSquare(glm::vec3(0.329, 0.321, 0.419));
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("girl");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters"), "girl.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, 0.01f, 200.0f);
    
    {
        auto maze = mMazeGenerator.GenerateMaze(10, 10, 1);
        vector<Mesh*> mazeMesh;

        for (int i = 0; i < 10; i++)
        {
            float r = (double)rand() / (RAND_MAX);
            float g = (double)rand() / (RAND_MAX);
            float b = (double)rand() / (RAND_MAX);
            meshes["box_maze_"s + to_string(i)] = CreateCube(glm::vec3(r, g, b));
            mazeMesh.push_back(meshes["box_maze_"s + to_string(i)]);
        }
        
        auto ans = mSpawnManager.GenerateMaze(mazeMesh, maze, glm::vec3(0, 1.5f, 0), glm::vec3(30, 2, 30), shaders["MyShader"]);
        mMazeObjs = ans.first;
        mValidLoc = ans.second;

        {
            meshes["player_head"] = CreateCube(glm::vec3(0.964, 0.905, 0.654));
            meshes["player_body"] = CreateCube(glm::vec3(0.329, 0.321, 0.419));
            meshes["player_hand"] = CreateCube(glm::vec3(0.274, 0.427, 0.235));
            meshes["player_leg"] = CreateCube(glm::vec3(0.176, 0.121, 0.360));

            meshes["ground"] = CreateCube(glm::vec3(0.494, 0.513, 0.427));

            vector<Mesh*> myVec{ meshes["player_head"] , meshes["player_body"] , meshes["player_hand"] , meshes["player_leg"] };

            int index = rand() % mValidLoc.size();
            glm::vec3 playerLoc = mValidLoc[index];

            mValidLoc.erase(mValidLoc.begin() + index);

            mPlayer = mSpawnManager.GeneratePlayer(myVec, shaders["MyShader"], glm::vec3(playerLoc.x, 1, playerLoc.z));
        }

        {
            meshes["life"] = CreateSquare(glm::vec3(0.886, 0.333, 0.529));
            meshes["time"] = CreateSquare(glm::vec3(0.105, 0.576, 0.419));
            meshes["wireframe"] = CreateWireFrameMesh(glm::vec3(0));
        }

        {
            int noEnemies = 4 + rand() % 4;

            for (int i = 0; i < noEnemies; i++)
            {
                int index = rand() % mValidLoc.size();
                glm::vec3 enemiyLoc = mValidLoc[index];

                mValidLoc.erase(mValidLoc.begin() + index);

                auto myGirl = make_shared<MyObjectT2>(meshes["girl"], Transform(glm::vec3(enemiyLoc.x, -1.0f, enemiyLoc.z), glm::vec3(1, 1, 1)),
                    Collision(glm::vec3(-1, 0, -1), glm::vec3(1, 1.5, 1), true), shaders["MyShader"]);

                myGirl->startPos = glm::vec3(enemiyLoc.x, 0, enemiyLoc.z);

                mEnemies.push_back(myGirl);
            }
        }

        mFinishLine = make_shared<MyObjectT2>(meshes["player_head"], Transform(glm::vec3(-14, 0.5, -14), glm::vec3(0.5)),
            Collision(glm::vec3(-14.5, 0, -14.5), glm::vec3(-13.5, 1, -13.5), true), shaders["MyShader"]);

        mGround = make_shared<MyObjectT2>(meshes["ground"], Transform(glm::vec3(0, -1, 0), glm::vec3(30, 0.1, 30)),
            Collision(glm::vec3(0), glm::vec3(0), false), shaders["MyShader"]);
    }
    window->DisablePointer();
}


void Tema2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    mTimeToSolve -= deltaTimeSeconds;
    if (mTimeToSolve < 0)
    {
        cout << "Game over!\n";
        exit(0);
    }

    myTime += deltaTimeSeconds;
    if (myTime > maxTime)
        myTime = 0;

    if (glm::length(mPlayer->mTransform.mVelocity) > 0.5) {
        mPlayer->UpdateAnimation(deltaTimeSeconds);
    }
    else {
        mPlayer->ResetAnimation();
    }
    mPlayer->UpdateTransform(deltaTimeSeconds);
    
    if (mFPV) {
        mCameraDir = glm::vec3(sin(mPlayer->mTransform.mRotationAngle.y) * sin(angleY), cos(angleY), cos(mPlayer->mTransform.mRotationAngle.y) * sin(angleY));
        camera->Set(mPlayer->mTransform.mPosition + glm::vec3(0, 1.8f, 0) + mDistCameraPlayer * 0.01f * mCameraDir, mPlayer->mTransform.mPosition + glm::vec3(0, 1.8f, 0), glm::vec3(0, 1, 0));
    }
    else {
        mCameraDir = glm::vec3(sin(mPlayer->mTransform.mRotationAngle.y) * sin(angleY), 1 + cos(angleY), cos(mPlayer->mTransform.mRotationAngle.y) * sin(angleY));
        camera->Set(mPlayer->mTransform.mPosition + mDistCameraPlayer * mCameraDir, mPlayer->mTransform.mPosition + glm::vec3(0, 1.8f, 0), glm::vec3(0, 1, 0));

        mPlayer->RenderObjMesh(camera->GetViewMatrix(), projectionMatrix);
    }
    
    glm::vec3 responseFinishLine = mPlayer->mCollider.IsColliding(mFinishLine->mCollider);
    if (responseFinishLine != glm::vec3(0))
    {
        cout << "You won in " << mMaxTime - mTimeToSolve << " second!\n";
        exit(0);
    }

    for (auto obj : mMazeObjs)
    {
        if (obj->mCollider.mIsActive)
        {
            glm::vec3 response = mPlayer->mCollider.IsColliding(obj->mCollider);

            if (response != glm::vec3(0))
            {
                mPlayer->mTransform.mVelocity = glm::vec3(0);
                mPlayer->SetAcceleration(50.0f * response);
            }
        }
    }

    for (auto proj = mProjs.begin(); proj != mProjs.end();)
    {
        bool collided = false;
        for (auto obj : mMazeObjs)
        {
            if (obj->mCollider.mIsActive)
            {
                glm::vec3 response = (*proj)->mCollider.IsColliding(obj->mCollider);

                if (response != glm::vec3(0))
                {
                    proj = mProjs.erase(proj);
                    collided = true;
                    break;
                }                    
            }
            
        }

        if (collided)
            continue;

        for (auto & enemy : mEnemies)
        {
            glm::vec3 response = (*proj)->mCollider.IsColliding(enemy->mCollider);

            if (response != glm::vec3(0))
            {
                proj = mProjs.erase(proj);

                enemy->mCountTime = true;
                enemy->mTimer = 0;

                collided = true;
                break;
            }
        }

        if(!collided)
            proj++;
    }

    for(auto& enemy : mEnemies)
    {
        glm::vec3 response = mPlayer->mCollider.IsColliding(enemy->mCollider);

        if (response != glm::vec3(0))
        {
            mHealth -= 20 * deltaTimeSeconds;
            if (mHealth < 0)
            {
                cout << "Game over!\n";
                exit(0);
            }

            mPlayer->mTransform.mVelocity = glm::vec3(0);
            mPlayer->SetAcceleration(50.0f * response);
        }
    }

    mFinishLine->RenderObjMesh(camera->GetViewMatrix(), projectionMatrix);
    mGround->RenderObjMesh(camera->GetViewMatrix(), projectionMatrix);

    for(auto proj : mProjs)
    {
        proj->UpdateTransform(deltaTimeSeconds);
        proj->RenderObjMesh(camera->GetViewMatrix(), projectionMatrix);
    }

    for (auto& mazeObj : mMazeObjs)
    {
        mazeObj->RenderObjMesh(camera->GetViewMatrix(), projectionMatrix);
    }

    for (auto enemy = mEnemies.begin(); enemy != mEnemies.end();)
    {
        (*enemy)->Wander(deltaTimeSeconds, glm::vec2(mPlayer->mTransform.mPosition.x, mPlayer->mTransform.mPosition.z));

        if ((*enemy)->UpdateTransform(deltaTimeSeconds))
        {
            enemy = mEnemies.erase(enemy);
            continue;
        }
        RenderMesh((*enemy)->mMesh[0], shaders["ExpShader"], (*enemy)->GetTransformMatrix(glm::vec3(0)), (*enemy)->mCountTime, (*enemy)->mTimer);
        enemy++;
    }

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, resolution.y * 0.7, resolution.x * 0.3, resolution.y * 0.3);

    glm::mat4 myMat = glm::mat4(1);

    myMat *= transform3D::Translate(0, 0.5, 0);
    myMat *= transform3D::Scale(0.8, 0.2, 0.2);

    RenderMesh(meshes["wireframe"], shaders["VertexNormal"], myMat, false, 10);

    myMat = glm::mat4(1);
    myMat *= transform3D::Translate(-0.785 * (1 - (float)mHealth / mMaxHealth), 0.5, 0);
    myMat *= transform3D::Scale(0.785 * ((float)mHealth / mMaxHealth), 0.185, 0.2);

    RenderMesh(meshes["life"], shaders["VertexNormal"], myMat, false, 10);

    myMat = glm::mat4(1);
    myMat *= transform3D::Translate(0, -0.1, 0);
    myMat *= transform3D::Scale(0.8, 0.2, 0.2);

    RenderMesh(meshes["wireframe"], shaders["VertexNormal"], myMat, false, 10);

    myMat = glm::mat4(1);
    myMat *= transform3D::Translate(-0.785 * (1 - (float)mTimeToSolve / mMaxTime), -0.1, 0);
    myMat *= transform3D::Scale(0.785 * ((float)mTimeToSolve / mMaxTime), 0.185, 0.2);

    RenderMesh(meshes["time"], shaders["VertexNormal"], myMat, false, 10);
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, bool anim, float time)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    if (time != 10)
    {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
    else
    {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr((glm::mat4(1))));
    }

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 randomMatrix(0);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            randomMatrix[i][j] = rand() % RAND_MAX + 1;
        }
    }

    if (shader == shaders["ExpShader"])
    {
        int location = glGetUniformLocation(shader->program, "RandomMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(randomMatrix));

        location = glGetUniformLocation(shader->program, "actTime");

        if(!anim)
            glUniform1f(location, 0);
        else
            glUniform1f(location, time);


        location = glGetUniformLocation(shader->program, "maxTime");
        glUniform1f(location, maxTime);
    }

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float airSlow = 3.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        float angle = mPlayer->mTransform.mRotationAngle.y;
        float traction = mPlayer->mTransform.mAccTraction;

        glm::vec3 acc{ traction * sin(angle), 0, traction * cos(angle) };

        mPlayer->SetAcceleration(glm::vec3(mPlayer->mTransform.mAcceleration.x, 0, 0) - acc);
        mPlayer->AddAcceleration(-airSlow * mPlayer->mTransform.mVelocity);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        float angle = mPlayer->mTransform.mRotationAngle.y;
        float traction = mPlayer->mTransform.mAccTraction;

        glm::vec3 acc{ traction * sin(angle), 0, traction * cos(angle) };

        mPlayer->SetAcceleration(glm::vec3(mPlayer->mTransform.mAcceleration.x, 0, 0) + acc);
        mPlayer->AddAcceleration(-airSlow * mPlayer->mTransform.mVelocity);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        float angle = mPlayer->mTransform.mRotationAngle.y;
        float traction = mPlayer->mTransform.mAccTraction;

        glm::vec3 acc{ traction * cos(angle), 0, -traction * sin(angle) };

        mPlayer->SetAcceleration(glm::vec3(0, 0, mPlayer->mTransform.mAcceleration.z) - acc);
        mPlayer->AddAcceleration(-airSlow * mPlayer->mTransform.mVelocity);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        float angle = mPlayer->mTransform.mRotationAngle.y;
        float traction = mPlayer->mTransform.mAccTraction;

        glm::vec3 acc{ traction * cos(angle), 0, -traction * sin(angle) };

        mPlayer->SetAcceleration(glm::vec3(0, 0, mPlayer->mTransform.mAcceleration.z) + acc);
        mPlayer->AddAcceleration(-airSlow * mPlayer->mTransform.mVelocity);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (window->GetSpecialKeyState())
        mFPV = !mFPV;

    if (key == GLFW_KEY_SPACE)
    {
        mProjs.push_back(mSpawnManager.SpawnProj(meshes["sphere"], mPlayer->mTransform.mPosition +
            glm::vec3(0, 1, 0), -mCameraDir, 10, glm::vec3(0.5, 0.5, 0.5), shaders["MyShader"]));
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mPlayer->mTransform.mRotationAngle.y -= 0.001 * deltaX;
    angleY -= 0.001 * deltaY;
    if (angleY < RADIANS(10))
        angleY = RADIANS(10);

    if (angleY > RADIANS(170))
        angleY = RADIANS(170);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_1))
    {
        mProjs.push_back(mSpawnManager.SpawnProj(meshes["sphere"], mPlayer->mTransform.mPosition +
            glm::vec3(0, 1, 0), -mCameraDir, 10, glm::vec3(0.5, 0.5, 0.5), shaders["MyShader"]));
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_2))
        mFPV = !mFPV;
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
