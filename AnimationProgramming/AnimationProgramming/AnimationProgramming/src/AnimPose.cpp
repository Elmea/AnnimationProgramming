#include "AnimPose.h"

#include "AnimSkeleton.h"
#include "../AnimBone.h"

void AnimPose::Init(const int& boneCount)
{
	this->bonesTransform.clear();
	this->bonesTransform.reserve(boneCount);

	this->boneCount = boneCount;
}

EmMaths::Mat4 AnimPose::GetBoneWorldPosition(const AnimSkeleton* skeleton , const int& boneIdx) const
{
	if (boneIdx <= 0)		//Root bone or error
	{
		return this->bonesTransform[0];
	}
	else
	{
		return GetBoneWorldPosition(skeleton, skeleton->skeletonBones.at(boneIdx).parentIndex) * this->bonesTransform[boneIdx];
	}
	return EmMaths::Mat4();
}
