// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include "stdafx.h"

#include "Engine.h"
#include "Simulation.h"
#include "Skeleton.h"

std::unique_ptr<Skeleton> skeleton;

class CSimulation : public ISimulation
{
	float nextAnimCounter = 0.f;
	float nextAnimDelay = 5.f;
	bool idAnim = true;

	virtual void Init() override
	{
		int spine01 =	GetSkeletonBoneIndex("spine_01");
		
		for (size_t i = 0; i < GetSkeletonBoneCount(); i++)
		{
			int parent = GetSkeletonBoneParentIndex(i);
			std::cout <<  GetSkeletonBoneName(i) << " is child of " << (parent == -1 ? "no parent " : GetSkeletonBoneName(parent)) << std::endl;
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
		GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, 0, posX, posY, posZ, quatW, quatX, quatY, quatZ);

		printf("Spine parent bone : %s\n", pelvisParentName);
		printf("Spine parent bone : %s\n", spineParentName);
		printf("Anim key count : %ld\n", keyCount);
		printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);
		
		skeleton = std::make_unique<Skeleton>(std::vector<std::string>{"ThirdPersonWalk.anim", "ThirdPersonRun.anim"});
		std::cout << "----------------------------------------------------- Display : " << std::endl;
		skeleton->displayConsolGraph();
		std::cout << "----------------------------------------------------- Draw : " << std::endl;

		std::cout << GetSkeletonBoneName(skeleton->data.id) << std::endl;
		skeleton->data.getGlobalMatrix().display();
		std::cout << "----------------------------------------------------- : " << std::endl;
		std::cout << GetSkeletonBoneName(skeleton->children.front().data.id) << std::endl;
		skeleton->children.front().data.getGlobalMatrix().display();
		
	}

	virtual void Update(float frameTime) override
	{
		nextAnimCounter += frameTime;

		if (nextAnimCounter >= nextAnimDelay)
		{
			nextAnimCounter -= nextAnimDelay;
			skeleton->blendAnimWithOther(idAnim, 3.f);
			idAnim = !idAnim;
		}

		skeleton->updateWithAnimation(frameTime);
		skeleton->draw();

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

