#pragma once

#include "Engine.h"
#include "MathsLib/EmMaths.hpp"
#include "src/Transform.h"

// return local transform (ie relative to the parent) of the bone in the bind pose (or rest pose, also called T-pose)
void GetSkeletonBoneLocalBindTransform(int boneIndex, EmMaths::Float3& position, EmMaths::Quaternion& quat);

// return the transform relative to the bind pose
void GetSkeletonBoneLocalBindTransform(int boneIndex, Transform& transform);

// there are two animations named "ThirdPersonWalk.anim" and "ThiedPersonRun.anim"
void GetAnimLocalBoneTransform(const char* animName, int boneIndex, int keyFrameIndex, EmMaths::Float3& transformPosition, EmMaths::Quaternion& transformQuat);
void GetAnimLocalBoneTransform(const char* animName, int boneIndex, int keyFrameIndex, Transform& transform);

void DrawLine(const EmMaths::Float3& startPosition, const EmMaths::Float3& endPosition, const EmMaths::Float3& color);
void DrawLine(const Transform& startPosition, const Transform& endPosition, const EmMaths::Float3& color);