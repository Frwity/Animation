// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <mutex>
#include "stdafx.h"

#include "Engine.h"
#include "Simulation.h"
#include "Skeleton.h"

std::unique_ptr<Skeleton> skeleton;
std::mutex mlock;

class CSimulation : public ISimulation
{
	bool idAnim = true;
	bool pause = false;
	bool edge = false;

	virtual void Init() override
	{
		mlock.lock();

		std::cout << "----------------------------------------------------- Construct : " << std::endl;
		skeleton = std::make_unique<Skeleton>(std::vector<std::string>{"ThirdPersonWalk.anim", "ThirdPersonRun.anim"});
		std::cout << "----------------------------------------------------- Display : " << std::endl;
		skeleton->displayBones();
		std::cout << "----------------------------------------------------- Draw : " << std::endl;

		mlock.unlock();
	}

	virtual void Update(float frameTime) override
	{
		mlock.lock();

		if (IsJustPressed(EInput::F1))
		{
			skeleton->animSpeed = 10.f;
			pause = !pause;
		}

		if (pause)
		{
			drawPauseLogo({60,230, 50}, 100, 1, 0, 0);
		}
		else
		{
			drawPlayLogo({60,230, 50}, 100, 0, 1, 0);
		}

		drawDigit({60,230, 50}, 50, 5, 1, 1, 0);
		drawEqualLogo({100,242.5, 50}, 50, 1, 1, 0);
		drawNumber({140,230, 50}, 50, std::abs((int)skeleton->animSpeed), 1, skeleton->animSpeed < 0.f ? 0 : 1, 0);


		if (IsJustPressed(EInput::F2))
		{
			skeleton->blendAnimWithOther(idAnim, 0.3f);
			idAnim = !idAnim;
		}

		if (IsJustPressed(EInput::F3))
			skeleton->animSpeed--;
		if (IsJustPressed(EInput::F4))
			skeleton->animSpeed++;

		if (IsJustPressed(EInput::F5))
		{
			edge = !edge;
			DrawEdge(edge);
		}

		if(!pause)
			skeleton->updateWithAnimation(frameTime);
		
		drawNumber({-60,230, 50}, 100, (int)skeleton->keyFrameCurrentAnim, 0, 0, 1);

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

		mlock.unlock();
	}

	//For 99 max
	void drawNumber(Vector3 pos, float size, int number, float r, float g, float b)
	{
		drawDigit(pos, size, number / 10, r, g, b);
		pos.x += size / 2 + size / 10;
		drawDigit(pos, size, number % 10, r, g, b);
	}

	void drawDigit(Vector3 pos, float size, int digit, float r, float g, float b)
	{
		switch (digit)
		{
		case 0:
			drawDigitULine(pos, size, r, g, b);
			drawDigitULLine(pos, size, r, g, b);
			drawDigitURLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 1:
			drawDigitURLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			break;
		case 2:
			drawDigitURLine(pos, size, r, g, b);
			drawDigitULine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBLLine(pos, size, r, g, b); 
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 3:
			drawDigitURLine(pos, size, r, g, b);
			drawDigitULine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 4:
			drawDigitURLine(pos, size, r, g, b);
			drawDigitULLine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			break;
		case 5:
			drawDigitULLine(pos, size, r, g, b);
			drawDigitULine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 6:
			drawDigitULine(pos, size, r, g, b);
			drawDigitULLine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 7:
			drawDigitULine(pos, size, r, g, b);
			drawDigitURLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			break;
		case 8:
			drawDigitULine(pos, size, r, g, b);
			drawDigitULLine(pos, size, r, g, b);
			drawDigitURLine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		case 9:
			drawDigitULine(pos, size, r, g, b);
			drawDigitURLine(pos, size, r, g, b);
			drawDigitULLine(pos, size, r, g, b);
			drawDigitMLine(pos, size, r, g, b);
			drawDigitBRLine(pos, size, r, g, b);
			drawDigitBLine(pos, size, r, g, b);
			break;
		default:
			break;
		}
	}

	void drawPauseLogo(Vector3 pos, float size, float r, float g, float b)
	{
		drawDigitULLine(pos, size, r, g, b);
		drawDigitURLine(pos, size, r, g, b);
	}

	void drawEqualLogo(Vector3 pos, float size, float r, float g, float b)
	{
		drawDigitBLine(pos, size, r, g, b);
		drawDigitMLine(pos, size, r, g, b);
	}

	void drawPlayLogo(Vector3 pos, float size, float r, float g, float b)
	{
		drawDigitULLine(pos, size, r, g, b);
		FlipYZDrawLine(0 + pos.x, 1.f * size  + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 0.75f* size, pos.z + 0 * size, r, g, b);
		FlipYZDrawLine(0 + pos.x, 0.5f * size  + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 0.75f* size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitULine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0 + pos.x, 1.f  * size  + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 1.f* size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitULLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0 + pos.x, 0.5f * size  + pos.y, 0.f + pos.z, pos.x + 0 * size, pos.y + 1.f * size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitURLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0.5f  * size  + pos.x, 0.5f  * size  + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 1.f * size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitMLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0 + pos.x, 0.5f  * size  + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 0.5f * size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitBLLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0 + pos.x, 0 + pos.y, 0 + pos.z, pos.x + 0.f * size, pos.y + 0.5f * size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitBRLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0.5f * size + pos.x, 0 + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 0.5f * size, pos.z + 0 * size, r, g, b);
	}

	void drawDigitBLine(Vector3 pos, float size, float r, float g, float b)
	{
		FlipYZDrawLine(0 + pos.x, 0 + pos.y, 0 + pos.z, pos.x + 0.5f * size, pos.y + 0 * size, pos.z + 0 * size, r, g, b);
	}

	void FlipYZDrawLine(float x0, float y0, float z0, float x1, float y1, float z1, float r, float g, float b)
	{
		DrawLine(x0, z0, y0, x1, z1, y1, r, g, b);
	}
};

int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

    return 0;
}