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
	}

	Quaternion a {Quaternion::AngleAxis({0, 1, 0}, MY_PI/2)};
	Quaternion b {Quaternion::AngleAxis({0, 1, 0}, 0)};
	Quaternion c {Quaternion::AngleAxis({0.4f, 0.72f, 0.15f}, MY_PI/2).GetNormalized()};
	//Quaternion c {Quaternion::AngleAxis({0, 0, 0.5}, MY_PI/2)};
	
	Float3 testRoot {- 100, 0, 0};
	Float4 testVec {- 100, 0, 0};
	Float3 NTestRoot {- 100, 50, 0};
	Float3 CTestRoot {- 250, 25, 0};

	bool timerGoesUp;
	float SLerptimer;
	const float timerLimit = 5.f;

	void DrawLerpTest(float frameTime)
	{
		DrawLine(testRoot, testRoot + (testVec * a.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		DrawLine(testRoot, testRoot + (testVec * b.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		Quaternion curr = Quaternion::SLerp(a, b, SLerptimer/timerLimit);
		DrawLine(testRoot, testRoot + (testVec * curr.GetRotationMatrix()).getXYZF3(), Float3{0.75, 0, 1});

		DrawLine(NTestRoot, NTestRoot + (testVec * a.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		DrawLine(NTestRoot, NTestRoot + (testVec * b.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		curr = Quaternion::NLerp(a, b, SLerptimer/timerLimit);
		DrawLine(NTestRoot, NTestRoot + (testVec * curr.GetRotationMatrix()).getXYZF3(), Float3{1.f, 0.6f, 0.1f});

		DrawLine(CTestRoot, CTestRoot + (testVec * b.GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		DrawLine(CTestRoot, CTestRoot + (testVec * c.GetNormalized().GetRotationMatrix()).getXYZF3(), Float3{0, 0, 0});
		curr = Quaternion::SLerp(b, c, SLerptimer/timerLimit);
		DrawLine(CTestRoot, CTestRoot + (testVec * curr.GetRotationMatrix()).getXYZF3(), Float3{0.2f, 0.6f, 0.2f});
		
		if (timerGoesUp)
		{
			SLerptimer += frameTime;
			if (SLerptimer >= timerLimit)
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

		//skeleton.DrawSkeletonBindPose();
		//DrawLerpTest(frameTime);

		AnimPose skeletonPose = skeleton.ComputeAnimatedPose();
		skeleton.DrawAnimPose(skeletonPose);
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
