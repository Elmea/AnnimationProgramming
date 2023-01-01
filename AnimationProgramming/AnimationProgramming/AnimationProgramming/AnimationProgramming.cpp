// AnimationProgramming.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "Engine.h"
#include "Macros.h"
#include "Simulation.h"

#include "src/AnimSkeleton.h"
#include "AnimBone.h"

#include "MathsLib/EmMaths.hpp"
#include "imgui/imgui.h"

#include <vector>

using namespace EmMaths;

class CSimulation : public ISimulation
{
	AnimSkeleton skeleton;

	Quaternion a {Quaternion::AngleAxis({0, 1, 0}, MY_PI/2)};
	Quaternion b {Quaternion::AngleAxis({0, 1, 0}, 0)};
	Quaternion c {Quaternion::AngleAxis({0.4f, 0.72f, 0.15f}, 3*MY_PI/4).GetNormalized()};
	
	Float3 testRoot {- 100, 0, 0};
	Float4 testVec {- 100, 0, 0};
	Float3 NTestRoot {- 100, 50, 0};
	Float3 CTestRoot {- 250, 25, 0};
	
	virtual void Init() override
	{
		ImGui::SetCurrentContext((ImGuiContext*)GetImGUIContext());

		skeleton.InitSkeleton();
		bonesWorldPos.reserve(skeleton.boneCount);
	}

	void DrawLerpTest(const float& frameTime)
	{
		static bool timerGoesUp;
		static float SLerptimer;
		static const float timerLimit = 5.f;

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

	std::vector<Mat4> bonesWorldPos;
	
	virtual void Update(float frameTime) override
	{
		CSimulation::EngineDrawGizmo();

		skeleton.DrawSkeletonBindPose();
		//DrawLerpTest(frameTime);

		AnimPose skeletonPose = skeleton.ComputeAnimatedPose(frameTime, 1, true);

		skeleton.DrawAnimPose(skeletonPose, {-200,0,0});
		
		//TODO : Clean this and make it in a proper way
		bonesWorldPos.clear();
		
		for (int i = 0; i < skeleton.boneCount; i++)
			bonesWorldPos.push_back((skeletonPose.GetBoneWorldPosition(&skeleton, i) * skeleton.bindPose.GetBoneWorldPosRecursif(&skeleton, i).getInverseMatrix()).getTransposedMatrix()); 
		
		SetSkinningPose(bonesWorldPos[0].AsPtr(), skeleton.boneCount);
		skeleton.DrawAnimPose(skeletonPose, {-200,0,0});
	}

	static void EngineDrawGizmo()
	{	
		//Static huh ?

		static const Float3 zero;

		static const Float3 red(1, 0, 0);
		static const Float3 green(0, 1, 0);
		static const Float3 blue(0, 0, 1);

		static const Float3 xAxis(100, 0, 0);
		static const Float3 yAxis(0, 100, 0);
		static const Float3 zAxis(0, 0, 100);

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
