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
		std::cout << "----------------------------------------------------- Construct : " << std::endl;
		skeleton = std::make_unique<Skeleton>(std::vector<std::string>{"ThirdPersonWalk.anim", "ThirdPersonRun.anim"});
		std::cout << "----------------------------------------------------- Display : " << std::endl;
		skeleton->displayBones();
		std::cout << "----------------------------------------------------- Draw : " << std::endl;		
	}

	virtual void Update(float frameTime) override
	{
		nextAnimCounter += frameTime;

		if (nextAnimCounter >= nextAnimDelay)
		{
			nextAnimCounter -= nextAnimDelay;
			skeleton->blendAnimWithOther(idAnim, 0.3f);
			idAnim = !idAnim;
		}
		
		skeleton->updateWithAnimation(frameTime);
		skeleton->draw();

		std::vector<float> matrixList;

		for (std::unique_ptr<Bone>& bone : skeleton->bones)
		{
			Matrix4 invTPose = Matrix4::identity();
			bone->getTPoseGlobalMatrix().inverse(invTPose);

			for (int i = 0; i < 16; ++i)
			{
				matrixList.push_back(((bone->getGlobalMatrix() * invTPose.transpose())).get(i));
			}
		}

		SetSkinningPose(matrixList.data(), skeleton->bones.size());

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

