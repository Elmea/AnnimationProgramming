#include "AnimSkeleton.h"

#include "../Macros.h"
#include "../Engine.h"
#include "../AnimBone.h"

#include "../MathsLib/EmMaths.hpp"

#include <iostream>

void AnimSkeleton::InitSkeleton()
{
	boneCount = GetSkeletonBoneCount();

	printf("Bones count : %d\n", boneCount);

	//skeletonBones.reserve(boneCount); //Crash Somehow try to reserve 8k bones ?

	for (size_t i = 0; i < boneCount; i++)
	{
		AnimBone newBone;

		newBone.index = i;
		newBone.name = GetSkeletonBoneName(i);
		newBone.parentIndex = GetSkeletonBoneParentIndex(i);

		std::cout << "Bone " << newBone.index << " : Name : " << newBone.name << " : Index Parent Bone : " << newBone.parentIndex << std::endl; //Printf cause crash

		skeletonBones.push_back(newBone);
	}
}

void AnimSkeleton::DrawSkeletonInEngine()
{
	const EmMaths::Float3 drawOffset = { 200,0,0 }; //TODO : Imgui parameters

	EmMaths::Float3 drawColor = { 1,0,1 };

	for (const AnimBone& bone : skeletonBones)
	{
		if (bone.parentIndex < 0)
		{
			continue;
		}

		EmMaths::Float3 bonePos;
		EmMaths::Mat4 boneMat = GetBoneWorldPosRecursif(bone.index);

		bonePos.x = boneMat.mat[0][3];
		bonePos.y = boneMat.mat[1][3];
		bonePos.z = boneMat.mat[2][3];

		EmMaths::Mat4 parentBoneMat = GetBoneWorldPosRecursif(bone.parentIndex);

		EmMaths::Float3 parentBonePos;

		parentBonePos.x = parentBoneMat.mat[0][3];
		parentBonePos.y = parentBoneMat.mat[1][3];
		parentBonePos.z = parentBoneMat.mat[2][3];

		bonePos += drawOffset;
		parentBonePos += drawOffset;

		DrawLine(bonePos, parentBonePos, drawColor);
	}
}

EmMaths::Mat4 AnimSkeleton::GetBoneWorldPosRecursif(const int& boneIndex)
{
	Transform boneLocalTransform;
	EmMaths::Mat4 boneWorldMat;
	AnimBone bone = this->skeletonBones.at(boneIndex);

	//Root ou autre
	if (boneIndex <= 0)
	{
		GetSkeletonBoneLocalBindTransform(0, boneLocalTransform);
		boneWorldMat = boneLocalTransform.GetTransformMatrix();
		return boneWorldMat;
	}
	else
	{
		GetSkeletonBoneLocalBindTransform(boneIndex, boneLocalTransform);
		boneWorldMat = boneLocalTransform.GetTransformMatrix();
		return GetBoneWorldPosRecursif(bone.parentIndex) * boneWorldMat;
	}
}