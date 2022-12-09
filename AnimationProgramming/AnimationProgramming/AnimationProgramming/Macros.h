#pragma once

#include "Engine.h"
#include "MathsLib/EmMaths.hpp"

// return local transform (ie relative to the parent) of the bone in the bind pose (or rest pose, also called T-pose)
void GetSkeletonBoneLocalBindTransform(int boneIndex, EmMaths::Float3& position, EmMaths::Quaternion& quat) { GetSkeletonBoneLocalBindTransform(boneIndex, position.x, position.y, position.z, quat.d, quat.a, quat.b, quat.c); };

// return the transform relative to the bind pose
// there are two animations named "ThirdPersonWalk.anim" and "ThiedPersonRun.anim"
void GetAnimLocalBoneTransform(const char* animName, int boneIndex, int keyFrameIndex, EmMaths::Float3& transformPosition, EmMaths::Quaternion& transformQuat) { GetAnimLocalBoneTransform(animName, boneIndex, keyFrameIndex, transformPosition.x, transformPosition.y, transformPosition.z, transformQuat.d, transformQuat.a, transformQuat.b, transformQuat.c);};

void DrawLine(EmMaths::Float3& startPosition, EmMaths::Float3& endPosition, EmMaths::Float3& color) { DrawLine(startPosition.x , startPosition.y, startPosition.z, endPosition.x, endPosition.y, endPosition.z, color.x, color.y, color.z); };