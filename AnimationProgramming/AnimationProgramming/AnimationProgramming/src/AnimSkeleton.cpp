#include "AnimSkeleton.h"

#include "../Macros.h"
#include "../AnimBone.h"

#include "../MathsLib/EmMaths.hpp"

#include "KeyFrame.h"
#include "AnimPose.h"
#include "AnimClip.h"

#include <iostream>

void AnimSkeleton::InitSkeleton()
{
	this->InitBones();
	this->InitAnimClips();
}

void AnimSkeleton::DrawSkeletonBindPose()
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
		EmMaths::Mat4 boneMat = bindPose.GetBoneWorldPosRecursif(this, bone.index);

		bonePos.x = boneMat.mat[0][3];
		bonePos.y = boneMat.mat[1][3];
		bonePos.z = boneMat.mat[2][3];

		EmMaths::Mat4 parentBoneMat = bindPose.GetBoneWorldPosRecursif(this, bone.parentIndex);

		EmMaths::Float3 parentBonePos;

		parentBonePos.x = parentBoneMat.mat[0][3];
		parentBonePos.y = parentBoneMat.mat[1][3];
		parentBonePos.z = parentBoneMat.mat[2][3];

		bonePos += drawOffset;
		parentBonePos += drawOffset;

		DrawLine(bonePos, parentBonePos, drawColor);
	}
}

void AnimSkeleton::DrawAnimPose(const AnimPose& pose)
{
	//TODO : Imgui parameters
	EmMaths::Float3 drawOffset = {-200,0,0};
	static const EmMaths::Float3 drawColor = { 0,0,0 };

	//

	constexpr int boneCountOffset = 2;  //Start at 2 to not draw pelvis-root line

	for (int i = boneCountOffset; i < pose.boneCount; i++)
	{
		int parentIdex = this->skeletonBones[i].parentIndex;

		if (parentIdex < 0)
			continue;
		
		EmMaths::Mat4 boneMat = pose.GetBoneWorldPosition(this, i);
		EmMaths::Mat4 parentBoneMat = pose.GetBoneWorldPosition(this, parentIdex);

		EmMaths::Float3 bonePosition = { boneMat.mat[0][3], boneMat.mat[1][3], boneMat.mat[2][3] };
		EmMaths::Float3 parentBonePosition = { parentBoneMat.mat[0][3], parentBoneMat.mat[1][3], parentBoneMat.mat[2][3] };

		bonePosition += drawOffset;
		parentBonePosition += drawOffset;

		DrawLine(bonePosition, parentBonePosition, drawColor);
	}
}
	
AnimPose AnimSkeleton::ComputeAnimatedPose(const float &deltaTime)
{
	AnimPose finalPose;
	finalPose.Init(this->boneCount);

	constexpr int animClipIdx = 1;

	static const float keyFrameStep = this->animationsClips.at(animClipIdx).animDuration / this->animationsClips.at(animClipIdx).keyFrameCount;
	static const int keyFrameCount = this->animationsClips.at(animClipIdx).keyFrameCount;

	static float animTimer = 0;
	static int keyframeIdx = 0;

	animTimer += deltaTime;

	if (animTimer >= keyFrameStep)
	{
		animTimer = 0;
		keyframeIdx++;

		if (keyframeIdx >= keyFrameCount)
		{
			keyframeIdx = 0;
		}
	}

	//TODO : Keyframe by keyframe

	for (int i = 0; i < this->boneCount; i++)
	{
		EmMaths::Mat4 bindMatrice = this->bindPose.boneTransforms.at(i).GetTransformMatrix();
		EmMaths::Mat4 keyFrameMat = GetKeyFrame(this->animationsClips[animClipIdx], keyframeIdx).boneTransforms.at(i).GetTransformMatrix();

		EmMaths::Mat4 boneWorldPos = bindMatrice * keyFrameMat;

		finalPose.bonesTransform.push_back(boneWorldPos);
	}

	return finalPose;
}

void AnimSkeleton::InitBones()
{
	constexpr int bonesEnTrop = 7;

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
	this->animationsClips.clear();
	this->animationsClips.reserve(2);
	
	//Init bind pos
	bindPose = KeyFrame::GetBindPos(this->boneCount);

	//Init walk/run
	constexpr int animSampleRate = 30;	//! Magic number !

	AnimClip walkAnim(this, "ThirdPersonWalk.anim", animSampleRate);
	AnimClip runAnim(this, "ThirdPersonRun.anim", animSampleRate);

	this->animationsClips.push_back(walkAnim);
	this->animationsClips.push_back(runAnim);
}

KeyFrame AnimSkeleton::GetKeyFrame(const AnimClip& clip, const int& keyFrameIdx)
{
	return clip.keyFrames.at(keyFrameIdx);
}