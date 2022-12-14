// AnimationProgramming.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "Engine.h"
#include "Macros.h"
#include "Simulation.h"

#include "AnimBone.h"
#include "src/Transform.h"
#include "src/AnimSkeleton.h"

#include "MathsLib/EmMaths.hpp"

#include <iostream>

using namespace EmMaths;

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

	Quaternion a {Quaternion::AngleAxis({0, 1, 0}, MY_PI/2)};
	Quaternion b {Quaternion::AngleAxis({0, 1, 0}, 0)};

	Float3 testRoot {- 100, 0, 0};
	Float4 testFinal {- 100, 0, 0};
	Float3 NTestRoot {- 100, 50, 0};


	bool timerGoesUp;
	float SLerptimer;

	void DrawLerpTest(float frameTime)
	{
		DrawLine(testRoot, testRoot + (testFinal * a.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		DrawLine(testRoot, testRoot + (testFinal * b.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});

		DrawLine(NTestRoot, NTestRoot + (testFinal * a.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		DrawLine(NTestRoot, NTestRoot + (testFinal * b.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});

		Quaternion curr = Quaternion::SLerp(a, b, SLerptimer/5.f);
		DrawLine(testRoot, testRoot + (testFinal * curr.GetRotationMatrix()).getXYZF3(), Float3{0.75, 0, 1});

		curr = Quaternion::SLerp(a, b, SLerptimer/5.f);
		DrawLine(NTestRoot, NTestRoot + (testFinal * curr.GetRotationMatrix()).getXYZF3(), Float3{1.f, 0.6f, 0.1f});

		
		if (timerGoesUp)
		{
			SLerptimer += frameTime;
			if (SLerptimer >= 5)
			{
				timerGoesUp = false;
			}
		}
		else
		{
			SLerptimer -= frameTime;
			if (SLerptimer <= 0)
			{
				timerGoesUp = true;
			}
		}
	}
	
	virtual void Update(float frameTime) override
	{
		CSimulation::EngineDrawGizmo();
		skeleton.DrawSkeletonInEngine();
		DrawLerpTest(frameTime);
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
