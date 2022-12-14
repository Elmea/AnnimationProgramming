#pragma once

#include "AnimPose.h"

#include <vector>

class AnimClip
{
public :
	std::vector<AnimPose> keyFrames;

	float animDuration = 0.0f;	//In seconds
	int sampleRate = 0;			//Keyframes per sec		//30 in this project

public :
	AnimClip();
	~AnimClip();

	AnimClip(class AnimSkeleton* sekeleton, const char* animName, const int& sampleRate_);

public :

	void Init(class AnimSkeleton* sekeleton, const char* animName, const int &sampleRate_);
};

