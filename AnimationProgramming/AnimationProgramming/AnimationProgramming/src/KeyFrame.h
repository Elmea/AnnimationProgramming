#pragma once

#include "Transform.h"

#include <vector>

class KeyFrame
{
public :
	std::vector<Transform> boneTransforms;
	int keyFrameIndex = 0;
	int boneCount = 0;

public :
	void Init(const char* animName, const int& boneCount, const int& keyFrameIdx);
	void Init(const int& boneCount);

	static KeyFrame GetBindPos(const int& boneCount);

	EmMaths::Mat4 GetBoneWorldPosRecursif(const class AnimSkeleton* skeleton, const int& boneIndex);
};