#pragma once
#include "components/simple_scene.h"
#include "lab_m1/Tema_2/Animation.h"
#include "lab_m1/Tema_2/Collision.h"
using namespace std;

namespace m1
{
    struct Transform {
        glm::vec3 mPosition{ 0 };
        glm::vec3 mVelocity{ 0 };
        glm::vec3 mAcceleration{ 0 };

        double mAccTraction{ 10 };

        glm::vec3 mScale{ 1 };
        glm::vec3 mRotationAngle{ 0 };
        glm::vec3 mRotationSpeed{ 0 };

        double mDragFactor{ 0 };

        Transform() {};
        Transform(glm::vec3 aPossition, glm::vec3 aScale) : mPosition(aPossition), mScale(aScale) {}
        Transform(glm::vec3 aPossition, glm::vec3 aVelocity, glm::vec3 aScale, glm::vec3 aRotationSpeed,
            double aDragFactor) : mPosition(aPossition), mVelocity(aVelocity), mScale(aScale),
            mRotationSpeed(aRotationSpeed), mDragFactor(aDragFactor) {}
    };
    class MyObjectT2
    {
    public:
        Transform mTransform;
        Collision mCollider;

        vector<Mesh*> mMesh;
        vector<glm::vec3> mDisplacement;
        vector<glm::vec3> mScale;
        vector<glm::vec3> mRot;
        vector<Animation *> mAnimations;
        glm::vec3 startPos{glm::vec3(0)};

        Shader* mShader;

        float mTimer{ 0 };
        bool mCountTime{ false };

        MyObjectT2(Mesh* & aMesh, Transform& aTransform, Collision & aCollision, Shader * & aShader) : mMesh(),
            mTransform(aTransform), mCollider(aCollision), mShader(aShader) {
            mMesh.push_back(aMesh);
            mDisplacement.push_back(glm::vec3(0));
            mScale.push_back(glm::vec3(1));
            mRot.push_back(glm::vec3(0));
        };

        MyObjectT2(vector<Mesh*> & aMesh, vector<glm::vec3> & aDisplacement, vector<glm::vec3>
            & aScale, vector<glm::vec3> & aRot, Transform & aTransform, Collision & aCollision, Shader*& aShader,
            vector<Animation *> aAnimations) : mMesh(aMesh), mTransform(aTransform), mDisplacement(aDisplacement),
            mScale(aScale), mRot(aRot), mCollider(aCollision), mShader(aShader), mAnimations(aAnimations) {};

        glm::mat4 GetTransformMatrix(glm::vec3 aDisForRot);
        bool UpdateTransform(float deltaTimeSeconds);
        void UpdateAnimation(float deltaTimeSeconds);
        void ResetAnimation();

        void SetAcceleration(glm::vec3 aAcc);
        void AddAcceleration(glm::vec3 aAcc);

        void RenderObjMesh(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

        void Wander(float deltaTime, glm::vec2 mPlayerPos);
        //void LookAtAndFollow(glm::vec2 aPosition, float aMaxSpeed);
    };
}   // namespace m1
