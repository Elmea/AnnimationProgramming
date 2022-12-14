#pragma once

#include "../MathsLib/EmMaths.hpp"

#include <vector>

class AnimPose
{
public:
	std::vector<EmMaths::Mat4> bonesWorldPositions;

	int boneCount = 0;

public :
	void Init(const int& boneCount);

	EmMaths::Mat4 GetBoneWorldPosition(const class AnimSkeleton*skeleton, const int& boneIdx)const;
};