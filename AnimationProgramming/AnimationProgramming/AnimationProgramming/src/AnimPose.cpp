#include "AnimPose.h"

#include "Transform.h"

#include "../Macros.h"

void AnimPose::Init(const char* animName, const int& boneCount, const int& keyFrameIdx)
{
	this->boneCount = boneCount;
	this->keyFrameIndex = keyFrameIdx;

	//In case there was a previous use of this object
	this->boneTransforms.empty();
	this->boneTransforms.reserve(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		Transform boneTransform;
		GetAnimLocalBoneTransform(animName, i, keyFrameIdx, boneTransform);

		this->boneTransforms.push_back(boneTransform);
	}

	this->boneTransforms.shrink_to_fit();
}