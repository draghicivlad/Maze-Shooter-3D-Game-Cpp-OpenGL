#include "Collision.h"
#include <vector>
#include <iostream>
#include <optional>

using namespace std;
using namespace m1;

Collision::Collision(glm::vec3 aMin, glm::vec3 aMax, bool aIsActive) : mIsActive(aIsActive), mMin(aMin), mMax(aMax)
{
	mColliderType = ColliderType::AABB;
	mSize = (aMax - aMin) / 2.0f;
}

/*
Collision::Collision(glm::vec2 aCircleCenter, double aCircleRadius, bool aIsActive)
{
	isActive = aIsActive;
	mColliderType = ColliderType::Circle;
	mCircleCenter = aCircleCenter;
	mCircleRadius = aCircleRadius;
}
*/

glm::vec3 Collision::IsColliding(Collision other)
{
	if (mIsActive == false)
		return glm::vec3(0);

	if (this->mColliderType == ColliderType::AABB && other.mColliderType == ColliderType::AABB)
	{
		bool collisionX = (mMin.x <= other.mMax.x) && (mMax.x >= other.mMin.x);
		bool collisionY = (mMin.y <= other.mMax.y) && (mMax.y >= other.mMin.y);
		bool collisionZ = (mMin.z <= other.mMax.z) && (mMax.z >= other.mMin.z);

		if (collisionX && collisionY && collisionZ)
		{
			float deltaX = 0, deltaY = 0, deltaZ = 0;
			int sgnX = -1, sgnY = -1, sgnZ = -1;

			if (mMin.x > other.mMin.x)
			{
				deltaX = other.mMax.x - this->mMin.x;
				sgnX = 1;
			}
			else
				deltaX = this->mMin.x - other.mMax.x;
			//
			if (mMin.y > other.mMin.y)
			{
				deltaY = other.mMax.y - this->mMin.y;
				sgnY = 1;
			}
			else
				deltaY = this->mMin.y - other.mMax.y;
			//
			if (mMin.z > other.mMin.z)
			{
				deltaZ = other.mMax.z - this->mMin.z;
				sgnZ = 1;
			}
			else
				deltaZ = this->mMin.z - other.mMax.z;

			float minDelta = fmin(deltaX, fmin(deltaY, deltaZ));

			if (deltaX == minDelta)
				return glm::vec3(sgnX, 0, 0);
			if (deltaY == minDelta)
				return glm::vec3(0, 0, 0);
			if (deltaZ == minDelta)
				return glm::vec3(0, 0, sgnZ);
		}

		return glm::vec3(0);
	}
	return glm::vec3(0);
	/*
	if (this->mColliderType == ColliderType::Circle && other.mColliderType == ColliderType::Circle)
	{
		if (glm::length(this->mCircleCenter - other.mCircleCenter) < this->mCircleRadius + other.mCircleRadius)
			return glm::normalize(this->mCircleCenter - other.mCircleCenter);
		else
			return glm::vec2(0, 0);
	}

	Collision boxC, circleC;
	if (this->mColliderType == ColliderType::AABB)
	{
		boxC = *this;
		circleC = other;
	}

	if (this->mColliderType == ColliderType::Circle)
	{
		boxC = other;
		circleC = *this;
	}

	glm::vec2 aabb_half_extents(boxC.mSize.x, boxC.mSize.y);
	glm::vec2 aabb_center(
		boxC.mLeftDownCorner.x + aabb_half_extents.x,
		boxC.mLeftDownCorner.y + aabb_half_extents.y
	);

	glm::vec2 difference = circleC.mCircleCenter - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - circleC.mCircleCenter;

	if (glm::length(difference) > circleC.mCircleRadius)
		return glm::vec2(0, 0);
	else
		return -glm::normalize(difference);
	
	return glm::vec2(0, 0);
	*/
}
