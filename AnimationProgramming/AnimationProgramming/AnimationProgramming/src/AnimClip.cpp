#include "AnimClip.h"

#include "../Macros.h"

#include "KeyFrame.h"
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

	this->keyFrameCount = keyFramesCount;

	this->animName = animName;

	this->keyFrames.clear();
	this->keyFrames.reserve(keyFramesCount);

	this->sampleRate = sampleRate_;
	this->animDuration = (float)keyFramesCount/ (float)sampleRate_;
	
	for (int i = 0; i < keyFramesCount; i++)
	{
		KeyFrame keyframe;
		keyframe.Init(animName, sekeleton->boneCount, i);
		this->keyFrames.push_back(keyframe);	//should not cause a lot of copy thanks to reserve
	}
}