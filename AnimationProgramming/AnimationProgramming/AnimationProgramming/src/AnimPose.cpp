#include "AnimPose.h"

#include "AnimSkeleton.h"
#include "../AnimBone.h"

void AnimPose::Init(const int& boneCount)
{
	this->bonesWorldPositions.clear();
	this->bonesWorldPositions.reserve(boneCount);

	this->boneCount = boneCount;
}

EmMaths::Mat4 AnimPose::GetBoneWorldPosition(const AnimSkeleton* skeleton , const int& boneIdx) const
{
	if (boneIdx <= 0)		//Root bone or error
	{
		return this->bonesWorldPositions[0];
	}
	else
	{
		return GetBoneWorldPosition(skeleton, skeleton->skeletonBones.at(boneIdx).parentIndex) * this->bonesWorldPositions[boneIdx];
	}
	return EmMaths::Mat4();
}
