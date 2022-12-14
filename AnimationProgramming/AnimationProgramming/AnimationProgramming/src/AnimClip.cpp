#include "AnimClip.h"

#include "../Engine.h"

#include "AnimPose.h"
#include "AnimSkeleton.h"

AnimClip::AnimClip()
{
}

AnimClip::~AnimClip()
{
}

AnimClip::AnimClip(AnimSkeleton* sekeleton, const char* animName, const int& sampleRate_)
{
	this->Init(sekeleton, animName, sampleRate_);
}

void AnimClip::Init(AnimSkeleton* sekeleton, const char* animName, const int& sampleRate_)
{
	size_t keyFramesCount = GetAnimKeyCount(animName);

	this->keyFrames.clear();
	this->keyFrames.reserve(keyFramesCount);

	this->sampleRate = sampleRate_;
	this->animDuration = (float)keyFramesCount/ (float)sampleRate_;
	
	for (int i = 0; i < keyFramesCount; i++)
	{
		AnimPose keyframe;
		keyframe.Init(animName, sekeleton->boneCount, i);
		this->keyFrames.push_back(keyframe);	//should not cause a lot of copy thanks to reserve
	}
}