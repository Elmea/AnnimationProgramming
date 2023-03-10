#include "Macros.h"

// return local transform (ie relative to the parent) of the bone in the bind pose (or rest pose, also called T-pose)
void GetSkeletonBoneLocalBindTransform(int boneIndex, EmMaths::Float3& position, EmMaths::Quaternion& quat) { GetSkeletonBoneLocalBindTransform(boneIndex, position.x, position.y, position.z, quat.a, quat.b, quat.c, quat.d); };

void GetSkeletonBoneLocalBindTransform(int boneIndex, Transform& transform) { GetSkeletonBoneLocalBindTransform(boneIndex, transform.position, transform.rotation); };
// return the transform relative to the bind pose
// there are two animations named "ThirdPersonWalk.anim" and "ThiedPersonRun.anim"
void GetAnimLocalBoneTransform(const char* animName, int boneIndex, int keyFrameIndex, EmMaths::Float3& transformPosition, EmMaths::Quaternion& transformQuat) { GetAnimLocalBoneTransform(animName, boneIndex, keyFrameIndex, transformPosition.x, transformPosition.y, transformPosition.z, transformQuat.a, transformQuat.b, transformQuat.c, transformQuat.d); };

void GetAnimLocalBoneTransform(const char* animName, int boneIndex, int keyFrameIndex, Transform& transform) { GetAnimLocalBoneTransform(animName, boneIndex, keyFrameIndex, transform.position, transform.rotation); };

void DrawLine(const EmMaths::Float3& startPosition, const EmMaths::Float3& endPosition,const EmMaths::Float3& color) { DrawLine(startPosition.x, startPosition.y, startPosition.z, endPosition.x, endPosition.y, endPosition.z, color.x, color.y, color.z); };

void DrawLine(const Transform& startPosition, const Transform& endPosition, const EmMaths::Float3& color) { DrawLine(startPosition.position, endPosition.position, color); };