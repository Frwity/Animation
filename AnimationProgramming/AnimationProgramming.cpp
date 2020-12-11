// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"

#include "Engine.h"
#include "Simulation.h"
#include "Skeleton.h"

class CSimulation : public ISimulation
{
	virtual void Init() override
	{
		int spine01 =	GetSkeletonBoneIndex("spine_01");

		for (size_t i = 0; i < GetSkeletonBoneCount(); i++)
		{
			int parent = GetSkeletonBoneParentIndex(i);
			std::cout <<  (parent == -1 ? "no parent " : GetSkeletonBoneName(i)) << " is child of " << GetSkeletonBoneName(parent) << std::endl;
		}

		for (size_t i = 0; i < 3; i++)
		{
			std::cout << "-----------------------------------------------------" << std::endl;
		}

		int spineParent = GetSkeletonBoneParentIndex(spine01);
		const char* spineParentName = GetSkeletonBoneName(spineParent);
	

		int pelvis = GetSkeletonBoneIndex("pelvis");
		int pelvisParent = GetSkeletonBoneParentIndex(spine01);
		const char* pelvisParentName = GetSkeletonBoneName(spineParent);
		
		float posX, posY, posZ, quatW, quatX, quatY, quatZ;
		size_t keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
		GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, posX, posY, posZ, quatW, quatX, quatY, quatZ);

		printf("Spine parent bone : %s\n", pelvisParentName);
		printf("Spine parent bone : %s\n", spineParentName);
		printf("Anim key count : %ld\n", keyCount);
		printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);

		Skeleton skeleton;
		skeleton.displayConsolGraph();
		skeleton.draw();
	}

	virtual void Update(float frameTime) override
	{

		// X axis
		DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

		// Y axis
		DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

		// Z axis
		DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);

	}
};

int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

    return 0;
}

