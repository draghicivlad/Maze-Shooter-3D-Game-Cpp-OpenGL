#include "lab_m1/Tema_2/Animation.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema_2/transform3D.h"
#include "lab_m1/Tema_2/MyObjectT2.h"

using namespace std;
using namespace m1;

m1::Animation::Animation(glm::vec3 aRotDis, glm::vec3 aMinRot, glm::vec3 aMaxRot, float aDuration,
	glm::vec3 aStartRot, bool aMinToMax) : mAnimType(AnimType::Rotation), mRotDis(aRotDis), mMinRot(aMinRot),
	mMaxRot(aMaxRot), mDuration(aDuration), mActRot(aStartRot), mDefaultRot(aStartRot)
{
	mSpeed = (mMaxRot - mMinRot) / mDuration;
	mMinToMax = aMinToMax;
}

void m1::Animation::UpdateMe(float deltaTime, Transform & aTransform, glm::vec3& aDisplacement, glm::vec3& aScale, glm::vec3& aRot)
{
	float dir = mMinToMax ? 1 : -1;
	aRot += dir * mSpeed * deltaTime;

	bool xDiff = false, yDiff = false, zDiff = false;

	if (mSpeed.x != 0)
		xDiff = true;
	if (mSpeed.y != 0)
		yDiff = true;
	if (mSpeed.z != 0)
		zDiff = true;

	if (mSpeed.x != 0)
	{
		if (aRot.x > mMaxRot.x)
		{
			mMinToMax = false;
			return;
		}

		if (aRot.x < mMinRot.x)
		{
			mMinToMax = true;
			return;
		}
	}

	if (mSpeed.y != 0)
	{
		if (aRot.y > mMaxRot.y)
		{
			mMinToMax = false;
			return;
		}

		if (aRot.y < mMinRot.y)
		{
			mMinToMax = true;
			return;
		}
	}

	if (mSpeed.z != 0)
	{
		if (aRot.z > mMaxRot.z)
		{
			mMinToMax = false;
			return;
		}

		if (aRot.z < mMinRot.z)
		{
			mMinToMax = true;
			return;
		}
	}
}

void m1::Animation::Reset(glm::vec3& aRot)
{
	aRot = mDefaultRot;
}
