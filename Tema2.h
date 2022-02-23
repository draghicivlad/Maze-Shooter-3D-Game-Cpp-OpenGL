#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema_2/lab_camera.h"
#include "lab_m1/Tema_2/utilsT2.h"
#include "lab_m1/Tema_2/MyObjectT2.h"
#include "lab_m1/Tema_2/SpawnManager.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, bool anim, float time);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
         float myTime = 0;
         float maxTime = 1;

        implemented::Camera *camera;
        implemented::MazeGenerator mMazeGenerator;
        SpawnManager mSpawnManager;
        glm::mat4 projectionMatrix;

        shared_ptr<MyObjectT2> mPlayer;
        glm::vec3 mCameraDirInit = {0, 2, 1};
        glm::vec3 mCameraDir = {0, 2, 1};
        float mDistCameraPlayer = 2;
        float angleY = RADIANS(50);
        bool mFPV = false;


        shared_ptr<MyObjectT2> obj;

        shared_ptr<MyObjectT2> mFinishLine;
        shared_ptr<MyObjectT2> mGround;

        vector<shared_ptr<MyObjectT2>> mMazeObjs;
        vector<shared_ptr<MyObjectT2>> mProjs;

        vector<shared_ptr<MyObjectT2>> mEnemies;
        vector<glm::vec3> mValidLoc;

        float mHealth{ 100 };
        float mMaxHealth{ 100 };

        float mTimeToSolve{ 120 };
        float mMaxTime{ 120 };
        
        bool animate = false;

        bool isPers = true;
        float FOV = 60;
        float left = -10;
        float bottom = -10;

        float height = 20;
        float width = 20;
    };
}   // namespace m1
