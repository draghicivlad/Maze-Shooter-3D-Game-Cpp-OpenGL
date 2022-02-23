#pragma once
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
    struct Transform;
    class Animation
    {
    public:
        enum class AnimType {Rotation};

        Animation(glm::vec3 aRotDis, glm::vec3 aMinRot, glm::vec3 aMaxRot, float aDuration, glm::vec3 aStartRot, bool aMinToMax);
        void UpdateMe(float deltaTime, Transform & aTransform, glm::vec3 & aDisplacement, glm::vec3 & aScale, glm::vec3 & aRot);
        void Reset(glm::vec3& aRot);

        AnimType mAnimType;

        glm::vec3 mRotDis;
        glm::vec3 mMinRot;
        glm::vec3 mMaxRot;
        glm::vec3 mDefaultRot;
        float mDuration;

        glm::vec3 mActRot;

        glm::vec3 mSpeed;
        bool mMinToMax;

    };
}
