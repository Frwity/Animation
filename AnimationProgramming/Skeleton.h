#pragma once

#include <vector>
#include <iostream>

#include "Maths/Quaternion.hpp"
#include "Maths/matrix.hpp"
#include "Maths/vector3.hpp"
#include "Engine.h"

float lerp(float f1, float f2, float t)
{
	return f1 + t * (f2 - f1);
}

class Bone
{
public:

	const int id = -1;
	const Bone* const boneParent = nullptr;
	const Matrix4 TPoseMatrix{};
	std::vector<std::vector<Vector3>> animsLocalPos{}; //In function of animation Id and key Id
	std::vector<std::vector<Quaternion>> animsLocalRot{};

	Vector3 actualLocalPos{0.f, 0.f, 0.f};
	Quaternion actualLocalRot{0.f, 0.f, 0.f, 0.f};
	float scale = 1.f;

	Bone(Bone* _boneParent, int _id, const std::vector<std::string>& _anims, const Vector3& localBindPos, const Quaternion& localBindRot)
		: id{ _id }, boneParent{ _boneParent }, TPoseMatrix{ Matrix4::createTRSMatrix(localBindPos, localBindRot, Vector3(1.f, 1.f, 1.f))}
	{
		Vector3 newLocalPos;
		Quaternion newLocalRot;
		int animID = 0;
		int animCount = _anims.size();

		animsLocalPos.reserve(animCount);
		animsLocalRot.reserve(animCount);
		for (int i = 0; i < animCount; ++i)
		{
			animsLocalPos.emplace_back();
			animsLocalPos.back().reserve(GetAnimKeyCount(_anims[i].c_str()));

			animsLocalRot.emplace_back();
			animsLocalRot.back().reserve(GetAnimKeyCount(_anims[i].c_str()));
		}

		for (const std::string& anim : _anims)
		{
			for (size_t i = 0; i < GetAnimKeyCount(anim.c_str()); ++i)
			{
				GetAnimLocalBoneTransform(anim.c_str(), id, i, newLocalPos.x, newLocalPos.y, newLocalPos.z, newLocalRot.w, newLocalRot.x, newLocalRot.y, newLocalRot.z);
				animsLocalPos[animID].emplace_back(localBindPos + newLocalPos);
				animsLocalRot[animID].emplace_back(localBindRot * newLocalRot);
			}
			++animID;
		}
	}

	Matrix4 getGlobalMatrix() const
	{
		if (boneParent == nullptr)
			return Matrix4::identity();

		return boneParent->getGlobalMatrix() * Matrix4::createTRSMatrix(actualLocalPos, actualLocalRot, Vector3(scale, scale, scale));
	}

	Matrix4 getTPoseGlobalMatrix() const
	{
		if (boneParent == nullptr)
			return Matrix4::identity();

		return boneParent->getTPoseGlobalMatrix() * TPoseMatrix;
	}

	Vector3 getGlobalPostion() const
	{ 
		Matrix4 globalMatrix = getGlobalMatrix();

		Vector3 toReturn;

		toReturn.x = globalMatrix.accessor(3, 0);
		toReturn.y = globalMatrix.accessor(3, 1);
		toReturn.z = globalMatrix.accessor(3, 2);

		return toReturn;
	}
};

class Skeleton : public Bone
{
public : 
	std::vector<std::unique_ptr<Bone>> bones;

	float animSpeed = 15.f;
	int idCurrentAnim = 0;
	int idBlendedAnim = -1;
	float keyFrameCurrentAnim = 0.f;
	float fadDuration = 1.f;
	float fadCount = 0.f;

	Skeleton(const std::vector<std::string>& _animNames)
		: Bone(nullptr, 0, _animNames, Vector3(), Quaternion())
	{
		createBones(_animNames);
	}
	
	void createBones(const std::vector<std::string>& _animNames)
	{
		for (size_t i = 0; i < GetSkeletonBoneCount() - 7; ++i)
		{
			Vector3 localPos;
			Quaternion localQuat;
			GetSkeletonBoneLocalBindTransform(i, localPos.x, localPos.y, localPos.z, localQuat.w, localQuat.x, localQuat.y, localQuat.z);

			if (i == 0)
				bones.emplace_back(std::make_unique<Bone>(nullptr, (int)i, _animNames, localPos, localQuat));
			else
				bones.emplace_back(std::make_unique<Bone>(bones[GetSkeletonBoneParentIndex(i)].get(), (int)i, _animNames, localPos, localQuat));
		}
	}

	void displayBones()
	{
		for (const std::unique_ptr<Bone>& bone : bones)
		{
			std::cout << GetSkeletonBoneName(bone->id) << " is child of " << (bone->boneParent == nullptr ? "no parent " : GetSkeletonBoneName(bone->boneParent->id)) << std::endl;
		}
	}

	void blendAnimWithOther(int newIdAnim, float newFadDuration)
	{
		if (newIdAnim > animsLocalPos.size())
			return;

		fadDuration = newFadDuration;
		idBlendedAnim = newIdAnim;
	}

	void updateWithAnimation(float deltaTime)
	{
		if (idBlendedAnim != -1)
		{

		}
		else
		{
			keyFrameCurrentAnim += deltaTime * animSpeed;

			if (keyFrameCurrentAnim > animsLocalPos[idCurrentAnim].size())
			{
				keyFrameCurrentAnim -= (float)animsLocalPos[idCurrentAnim].size();
			}
		}

		float t = keyFrameCurrentAnim - (int)keyFrameCurrentAnim;
		Vector3 currentPos;
		Vector3 goalPos;

		Quaternion currentRot;
		Quaternion goalRot;

		for (std::unique_ptr<Bone>& bone : bones)
		{
			int goalNextKeyCurrentAnim = ((int)keyFrameCurrentAnim + 1) % (bone->animsLocalPos[idCurrentAnim].size());

			if (idBlendedAnim != -1)
			{
				//std::cout << "idCurrentAnim " << idCurrentAnim << " keyFrameCurrentAnim " << keyFrameCurrentAnim << " idBlendedAnim " << idBlendedAnim <<
				//	" keyFrameBlendedAnim " << keyFrameBlendedAnim << " fadCount / fadDuration " << fadCount / fadDuration << std::endl;

				currentPos = bone->animsLocalPos[idCurrentAnim][keyFrameCurrentAnim];

				goalPos = bone->animsLocalPos[idBlendedAnim][0];

				currentRot = bone->animsLocalRot[idCurrentAnim][keyFrameCurrentAnim];

				goalRot = bone->animsLocalRot[idBlendedAnim][0];

				bone->actualLocalPos = Vector3::lerp(currentPos, goalPos, fadCount / fadDuration);
				bone->actualLocalRot = Quaternion::SLerp(currentRot, goalRot, fadCount / fadDuration);
			}
			else
			{
				currentPos = bone->animsLocalPos[idCurrentAnim][keyFrameCurrentAnim];
				goalPos = bone->animsLocalPos[idCurrentAnim][goalNextKeyCurrentAnim];

				currentRot = bone->animsLocalRot[idCurrentAnim][keyFrameCurrentAnim];
				goalRot = bone->animsLocalRot[idCurrentAnim][goalNextKeyCurrentAnim];
			
				bone->actualLocalPos = Vector3::lerp(currentPos, goalPos, t);
				bone->actualLocalRot = Quaternion::SLerp(currentRot, goalRot, t);
			}
		}

		if (idBlendedAnim != -1)
		{
			fadCount += deltaTime;

			if (fadCount >= fadDuration)
			{
				keyFrameCurrentAnim = 0.f;
				idCurrentAnim = idBlendedAnim;
				idBlendedAnim = -1;
				fadCount = 0.0f;
			}
		}
	}
		
	inline
	const Bone& getRoot() const { return *this; }

	void draw()
	{
		for (const std::unique_ptr<Bone>& bone : bones)
		{
			if (bone->boneParent == nullptr || bone->boneParent->id <= 0)
				continue;

			const Vector3 parentglobalPos = bone->boneParent->getGlobalPostion();
			const Vector3 childGlobalPos = bone->getGlobalPostion();
			DrawLine(parentglobalPos.x, parentglobalPos.y, parentglobalPos.z, childGlobalPos.x, childGlobalPos.y, childGlobalPos.z, 1, 1, 0);
			drawPoint(childGlobalPos);
		}
	}

	void drawPoint(Vector3 pos)
	{
		DrawLine(pos.x, pos.y, pos.z, pos.x + 1.5f, pos.y, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y + 1.5f, pos.z, pos.x + 1.5f, pos.y, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y + 1.5f , pos.z + 1.5f, pos.x + 1.5f, pos.y + 1.5f, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y, pos.z + 1.5f, pos.x + 1.5f, pos.y + 1.5f, pos.z, 1, 0, 0);
	}
};