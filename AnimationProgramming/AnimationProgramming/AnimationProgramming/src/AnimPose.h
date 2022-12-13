#pragma once

#include "Transform.h"

#include <vector>

class AnimPose
{
public :
	std::vector<Transform> boneTransforms;
	int keyFrameIndex = 0;
	int boneCount = 0;

public :
	void Init(const char* animName, const int& boneCount, const int& keyFrameIdx);
};

