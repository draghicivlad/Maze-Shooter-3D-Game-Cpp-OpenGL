//#include "lab_m1/Tema_1/MyObject.h"

#include <vector>
#include <iostream>
#include "lab_m1/Tema_2/transform3D.h"
//
#include "MyObjectT2.h"


using namespace std;
using namespace m1;

glm::mat4 m1::MyObjectT2::GetTransformMatrix(glm::vec3 aDisForRot)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(mTransform.mPosition.x + aDisForRot.x, mTransform.mPosition.y
        + aDisForRot.y, mTransform.mPosition.z + aDisForRot.z);

    modelMatrix *= transform3D::RotateOX(mTransform.mRotationAngle.x);
    modelMatrix *= transform3D::RotateOY(mTransform.mRotationAngle.y);
    modelMatrix *= transform3D::RotateOZ(mTransform.mRotationAngle.z);

    modelMatrix *= transform3D::Translate(-aDisForRot.x, -aDisForRot.y, -aDisForRot.z);

    modelMatrix *= transform3D::Scale(mTransform.mScale.x, mTransform.mScale.y, mTransform.mScale.z);
    return modelMatrix;
}

void m1::MyObjectT2::AddAcceleration(glm::vec3 aAcc)
{
    mTransform.mAcceleration += aAcc;
    mTransform.mAcceleration = (float)mTransform.mAccTraction * glm::normalize(mTransform.mAcceleration);
}

void m1::MyObjectT2::SetAcceleration(glm::vec3 aAcc)
{
    mTransform.mAcceleration = (float)mTransform.mAccTraction * glm::normalize(aAcc);
}

bool m1::MyObjectT2::UpdateTransform(float deltaTimeSeconds)
{
    if (mCountTime)
        mTimer += deltaTimeSeconds;

    if (mTimer > 0.7)
        return true;

    mTransform.mVelocity += mTransform.mAcceleration * deltaTimeSeconds;
    mTransform.mPosition += mTransform.mVelocity * deltaTimeSeconds;

    mTransform.mRotationAngle += mTransform.mRotationSpeed * deltaTimeSeconds;
    mTransform.mAcceleration = -(float)mTransform.mDragFactor * mTransform.mVelocity;
    
    if (mCollider.mColliderType == ColliderType::AABB)
    {

        if (strcmp(mMesh[0]->GetMeshID(), "girl"))
        {
            mCollider.mMin.x = mTransform.mPosition.x - mCollider.mSize.x;
            mCollider.mMin.z = mTransform.mPosition.z - mCollider.mSize.z;

            mCollider.mMax.x = mTransform.mPosition.x + mCollider.mSize.x;
            mCollider.mMax.z = mTransform.mPosition.z + mCollider.mSize.z;
        }
        else {
            mCollider.mMin.x = mTransform.mPosition.x - mCollider.mSize.x + 0.5f;
            mCollider.mMin.z = mTransform.mPosition.z - mCollider.mSize.z + 0.5f;

            mCollider.mMax.x = mTransform.mPosition.x + mCollider.mSize.x + 0.5f;
            mCollider.mMax.z = mTransform.mPosition.z + mCollider.mSize.z + 0.5f;
        }

    }
    
    return false;
}


void m1::MyObjectT2::UpdateAnimation(float deltaTimeSeconds)
{
    int i = 0;
    for (auto & actAnim : mAnimations)
    {
        if(actAnim != nullptr)
            actAnim->UpdateMe(deltaTimeSeconds, mTransform, mDisplacement[i], mScale[i], mRot[i]);
        i++;
    }
}

void m1::MyObjectT2::ResetAnimation()
{
    int i = 0;
    for (auto& actAnim : mAnimations)
    {
        if (actAnim != nullptr)
            actAnim->Reset(mRot[i]);
        i++;
    }
}

void m1::MyObjectT2::RenderObjMesh(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    if (!mShader || !mShader->GetProgramID())
        return;

    for (int i = 0; i < mMesh.size(); i++) {
        glm::vec3 aDisForRot(0);

        if(mAnimations.size() == mMesh.size())
            if (mAnimations[i] != nullptr)
                aDisForRot = mAnimations[i]->mRotDis;

        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix *= transform3D::Translate(mTransform.mPosition.x , mTransform.mPosition.y, mTransform.mPosition.z);

        modelMatrix *= transform3D::RotateOX(mTransform.mRotationAngle.x);
        modelMatrix *= transform3D::RotateOY(mTransform.mRotationAngle.y);
        modelMatrix *= transform3D::RotateOZ(mTransform.mRotationAngle.z);

        modelMatrix *= transform3D::Translate(mDisplacement[i].x + aDisForRot.x, aDisForRot.y + mDisplacement[i].y, mDisplacement[i].z + aDisForRot.z);

        modelMatrix *= transform3D::RotateOX(mRot[i].x);
        modelMatrix *= transform3D::RotateOY(mRot[i].y);
        modelMatrix *= transform3D::RotateOZ(mRot[i].z);

        modelMatrix *= transform3D::Translate(-aDisForRot.x, -aDisForRot.y, -aDisForRot.z);

        modelMatrix *= transform3D::Scale(mScale[i].x * mTransform.mScale.x, mScale[i].y * mTransform.mScale.y, mScale[i].z * mTransform.mScale.z);

        glUseProgram(mShader->program);
        int location = glGetUniformLocation(mShader->program, "Model");

        // TODO(student): Set shader uniform "Model" to modelMatrix
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // TODO(student): Get shader location for uniform mat4 "View"
        location = glGetUniformLocation(mShader->program, "View");

        // TODO(student): Set shader uniform "View" to viewMatrix
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // TODO(student): Get shader location for uniform mat4 "Projection"
        location = glGetUniformLocation(mShader->program, "Projection");

        // TODO(student): Set shader uniform "Projection" to projectionMatrix
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint loc_object_color = glGetUniformLocation(mShader->program, "object_color");

        if(mMesh[i]->vertices.size() > 0)
            glUniform3fv(loc_object_color, 1, glm::value_ptr(mMesh[i]->vertices[0].color));
        else
        {
            float r = (double)rand() / (RAND_MAX);
            float g = (double)rand() / (RAND_MAX);
            float b = (double)rand() / (RAND_MAX);
            glm::vec3 color(r, g, b);

            glUniform3fv(loc_object_color, 1, glm::value_ptr(color));
        }

        // Draw the object
        glBindVertexArray(mMesh[i]->GetBuffers()->m_VAO);
        glDrawElements(mMesh[i]->GetDrawMode(), static_cast<int>(mMesh[i]->indices.size()), GL_UNSIGNED_INT, 0);
    }
}

void m1::MyObjectT2::Wander(float deltaTime, glm::vec2 mPlayerPos)
{
    //cout << mTransform.mPosition << endl;

    float distToPlayer = glm::length(glm::vec2(mTransform.mPosition.x, mTransform.mPosition.z) - mPlayerPos);
    float distToCenter = glm::length(glm::vec2(startPos.x, startPos.z) - glm::vec2(mTransform.mPosition.x, mTransform.mPosition.z));

    glm::vec3 dirToCenter = glm::normalize(startPos - mTransform.mPosition);
    glm::vec3 dirToPlayer = glm::normalize(glm::vec3(mPlayerPos.x, 0, mPlayerPos.y) - mTransform.mPosition);

    dirToCenter.y = 0;
    dirToPlayer.y = 0;

    if (distToPlayer > 3)
    {
        if (distToCenter > 1.5)
        {
            // return

            mTransform.mVelocity = 2.0f * dirToCenter;
        }
        else
        {
            if (rand() < RAND_MAX * 0.8 * deltaTime)
            {
                srand(time(NULL));
                float x = 2 * ((double)rand() / RAND_MAX) - 1;
                float z = 2 * ((double)rand() / RAND_MAX) - 1;

                mTransform.mVelocity = 2.0f * glm::vec3(x, 0, z);
            }
            // wander
        }
    }
    else
    {
        mTransform.mVelocity = 2.0f * dirToPlayer;
        // follow
    }

    float angle = atan2(mTransform.mVelocity.x, mTransform.mVelocity.z);
    mTransform.mRotationAngle.y = angle;
}