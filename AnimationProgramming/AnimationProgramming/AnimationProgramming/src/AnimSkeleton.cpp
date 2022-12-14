#include "AnimSkeleton.h"

#include "../Macros.h"
#include "../Engine.h"
#include "../AnimBone.h"

#include "../MathsLib/EmMaths.hpp"

#include <iostream>

void AnimSkeleton::InitSkeleton()
{
	this->InitBones();
	this->InitAnimClips();
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

	if (boneIndex <= 0) //Root bone or error
	{
		GetSkeletonBoneLocalBindTransform(0, boneLocalTransform);
		return boneLocalTransform.GetTransformMatrix();
	}
	else
	{
		AnimBone bone = this->skeletonBones.at(boneIndex);
		GetSkeletonBoneLocalBindTransform(boneIndex, boneLocalTransform);

		return GetBoneWorldPosRecursif(bone.parentIndex) * boneLocalTransform.GetTransformMatrix();
	}
}

void AnimSkeleton::InitBones()
{
	constexpr int bonesEnTrop = 4;

	this->boneCount = GetSkeletonBoneCount();
	this->boneCount -= bonesEnTrop;

	printf("Bones count : %d\n", this->boneCount);

	for (size_t i = 0; i < this->boneCount; i++)
	{
		AnimBone newBone;

		newBone.index = i;
		newBone.name = GetSkeletonBoneName(i);
		newBone.parentIndex = GetSkeletonBoneParentIndex(i);

		std::cout << "Bone " << newBone.index << " : Name : " << newBone.name << " : Index Parent Bone : " << newBone.parentIndex << std::endl;		//Printf cause crash, so it's time for the good old std::cout ^^

		this->skeletonBones.push_back(newBone);
	}
}

void AnimSkeleton::InitAnimClips()
{
	constexpr int animSampleRate = 30;	//Magic number :'(

	AnimClip walkAnim(this, "ThirdPersonWalk.anim", animSampleRate);
	AnimClip runAnim(this, "ThirdPersonRun.anim", animSampleRate);

	this->animationsClips.push_back(walkAnim);
	this->animationsClips.push_back(runAnim);
}