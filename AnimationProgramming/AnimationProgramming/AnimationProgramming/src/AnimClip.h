#pragma once

#include "AnimPose.h"

#include <vector>

class AnimClip
{
public :
	std::vector<AnimPose> keyFrames;

	float animDuration = 0.0f;	//In seconds
	int sampleRate = 0;			//Keyframes per sec		//30 in this project
};

