// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "Macros.h"
#include "MathsLib/EmMaths.hpp"
#include "Simulation.h"

#include "AnimBone.h"

#include "src/Transform.h"

#include <vector>
#include <iostream>

using namespace EmMaths;

class AnimSkeleton
{
public :
	std::vector<AnimBone> skeletonBones = std::vector<AnimBone>();
	size_t boneCount = 0;

public :
	void InitSkeleton()
	{
		boneCount = GetSkeletonBoneCount();

		printf("Bones count : %d\n", boneCount);

		//skeletonBones.reserve(boneCount); //Crash Somehow try to reserve 8k bones ?

		for (size_t i = 0; i < boneCount; i++)
		{
			AnimBone newBone;

			newBone.index = i;
			newBone.name = GetSkeletonBoneName(i);
			newBone.parentIndex = GetSkeletonBoneParentIndex(i);

			std::cout << "Bone " << newBone.index << " : Name : " << newBone.name << " : Index Parent Bone : " << newBone.parentIndex << std::endl; //Printf cause crash

			skeletonBones.push_back(newBone);
		}
	}

	void DrawSkeletonInEngine()
	{
		const Float3 drawOffset = {10,0,0}; //TODO : Imgui parameters

		Float3 drawColor = { 1,0,1 };

		for (const AnimBone& bone : skeletonBones)
		{
			if (bone.parentIndex < 0)
			{
				continue;
			}
				Transform boneTransform = GetBoneWorldPos(bone.index);
				Transform parentTransform = GetBoneWorldPos(bone.parentIndex);

				boneTransform.position += drawOffset;
				parentTransform.position += drawOffset;

				DrawLine(boneTransform, parentTransform, drawColor);
		}
	}

	//TODO : Do a recursive function
	Transform GetBoneWorldPos(const int& boneIndex)
	{
		Transform worldPos;

		AnimBone ParentBone = this->skeletonBones.at(boneIndex);

		while (ParentBone.index > 0)
		{
			Transform parentRelativePos;
			GetSkeletonBoneLocalBindTransform(ParentBone.index, parentRelativePos);
			worldPos = worldPos + parentRelativePos;

			ParentBone = skeletonBones.at(ParentBone.parentIndex);
		}

		return worldPos;
	}
};

class CSimulation : public ISimulation
{
	AnimSkeleton skeleton;

	virtual void Init() override
	{
		skeleton.InitSkeleton();
		//Float3 bonePos;
		//Quaternion boneQuat;

		//size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
		//GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, bonePos, boneQuat);
		
		//printf("Spine parent bone : %s\n", spineParentName);
		//printf("Anim key count : %ld\n", keyCount);
		//printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", bonePos.x, bonePos.y, bonePos.z, boneQuat.d, boneQuat.a, boneQuat.b, boneQuat.c);
	}

	virtual void Update(float frameTime) override
	{
		CSimulation::EngineDrawGizmo();
		skeleton.DrawSkeletonInEngine();
	}

	static void EngineDrawGizmo()
	{
		Float3 zero;

		Float3 red(1, 0, 0);
		Float3 green(0, 1, 0);
		Float3 blue(0, 0, 1);

		Float3 xAxis(100, 0, 0);
		Float3 yAxis(0, 100, 0);
		Float3 zAxis(0, 0, 100);

		// X axis
		DrawLine(zero, xAxis, red);

		// Y axis
		DrawLine(zero, yAxis, green);

		// Z axis
		DrawLine(zero, zAxis, blue);
	}
};

int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

    return 0;
}
