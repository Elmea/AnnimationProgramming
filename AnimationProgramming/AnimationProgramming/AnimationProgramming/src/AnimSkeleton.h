#pragma once

#include "../MathsLib/EmMaths.hpp"
#include "KeyFrame.h"
#include "AnimPose.h"
#include "AnimClip.h"

#include <vector>

class AnimSkeleton
{
public:
	std::vector<class AnimBone> skeletonBones = std::vector<AnimBone>();
	std::vector<AnimClip> animationsClips;

	KeyFrame bindPose;

	size_t boneCount = 0;

public:
	void InitSkeleton();

	void DrawSkeletonBindPose();
	void DrawAnimPose(const AnimPose& pose);

	AnimPose ComputeAnimatedPose(const float& deltaTime);

private:
	void InitBones();
	void InitAnimClips();

	KeyFrame GetKeyFrame(const AnimClip& clip, const int &keyFrameIdx);		//Legacy
};