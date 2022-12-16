#include "KeyFrame.h"

#include "AnimSkeleton.h"
#include "../AnimBone.h"

#include "../Macros.h"

void KeyFrame::Init(const char* animName, const int& boneCount, const int& keyFrameIdx)		//Get skeleton here ?
{
	this->Init(boneCount);

	this->keyFrameIndex = keyFrameIdx;

	for (int i = 0; i < boneCount; i++)
	{
		Transform boneTransform;
		GetAnimLocalBoneTransform(animName, i, keyFrameIdx, boneTransform);

		this->boneTransforms.push_back(boneTransform);
	}

	this->boneTransforms.shrink_to_fit();
}

void KeyFrame::Init(const int& boneCount)
{
	this->boneCount = boneCount;

	this->boneTransforms.clear();
	this->boneTransforms.reserve(boneCount);
}

KeyFrame KeyFrame::GetBindPos(const int& boneCount)
{
	KeyFrame bindPos;
	bindPos.Init(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		Transform boneTransform;
		GetSkeletonBoneLocalBindTransform(i ,boneTransform);

		bindPos.boneTransforms.push_back(boneTransform);
	}

	bindPos.boneTransforms.shrink_to_fit();
	
	return bindPos;
}


EmMaths::Mat4 KeyFrame::GetBoneWorldPosRecursif(const AnimSkeleton*skeleton, const int& boneIndex)
{
	if (boneIndex < 0)		//Root bone or error
	{
		return EmMaths::Mat4::getIndentityMatrix();
	}
	else
	{
		return GetBoneWorldPosRecursif(skeleton, skeleton->skeletonBones.at(boneIndex).parentIndex) * this->boneTransforms[boneIndex].GetTransformMatrix();
	}
}