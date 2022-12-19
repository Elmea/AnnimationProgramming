#pragma once

#include "KeyFrame.h"
#include "MathsLib/EmMaths.hpp"

#include <vector>

class AnimClip
{
public :
	std::vector<KeyFrame> keyFrames;

	float animDuration = 0.0f;	//In seconds
	int sampleRate = 0;			//The magic number !

	int keyFrameCount = 0;

	const char* animName;

public :
	AnimClip();
	~AnimClip();

	AnimClip(class AnimSkeleton* sekeleton, const char* animName, const int& sampleRate_);

public :

	void Init(class AnimSkeleton* sekeleton, const char* animName, const int &sampleRate_);
};

