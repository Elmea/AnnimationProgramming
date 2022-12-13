#pragma once

#include "../AnimBone.h"

#include "../MathsLib/EmMaths.hpp"

#include <vector>

class AnimSkeleton
{
public:
	std::vector<AnimBone> skeletonBones = std::vector<AnimBone>();
	size_t boneCount = 0;

public:
	void InitSkeleton();
	void DrawSkeletonInEngine();

	EmMaths::Mat4 GetBoneWorldPosRecursif(const int& boneIndex);
};