#pragma once
#include "components/simple_scene.h"

using namespace std;

namespace m1
{
    enum class ColliderType { AABB, Circle };
    class Collision
    {
    public:
        bool mIsActive{ false };

        ColliderType mColliderType;

        glm::vec3 mMin;
        glm::vec3 mMax;

        glm::vec3 mSize;


        glm::vec2 mCircleCenter;
        double mCircleRadius;
       
        Collision() {}
        Collision(glm::vec3 aMin, glm::vec3 aMax, bool aIsActive);
        //Collision(glm::vec2 aCircleCenter, double aCircleRadius, bool aIsActive);

        glm::vec3 IsColliding(Collision other);
    };
}   
