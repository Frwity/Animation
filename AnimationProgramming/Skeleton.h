#pragma once

#include <vector>
#include <iostream>

#include "Maths/Quaternion.hpp"
#include "Maths/matrix.hpp"
#include "Maths/vector3.hpp"
#include "Graph.h"
#include "Engine.h"

class Bone
{
public:

	int id = -1;
	Bone* boneParent = nullptr;
	Vector3 originPos{};
	Quaternion originRot{};
	Vector3 localDifPos{};
	Quaternion localDifRot{};
	float scale = 1.f;

	Matrix4 getGlobalMatrix() const
	{
		if (boneParent == nullptr)
			return Matrix4::identity();

		return boneParent->getGlobalMatrix() * Matrix4::createTRSMatrix(originPos + localDifPos, originRot * localDifRot, Vector3(1.f, 1.f, 1.f));
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

class Skeleton : public Graph<Bone>
{
public : 
	std::string animName;

	float animSpeed = 10.f;
	float key = 0;

	Skeleton(const std::string& _animName)
	{
		data.id = 0;
		animName = _animName;
		addChildRecursive(*this, data.id);
	}

	void addChildRecursive(Graph<Bone>& parent, int start)
	{
		for (size_t i = start; i < GetSkeletonBoneCount(); ++i)
		{
			if (GetSkeletonBoneParentIndex(i) == parent.data.id)
			{
				Vector3 localPos;
				Quaternion localQuat;
				GetSkeletonBoneLocalBindTransform(i, localPos.x, localPos.y, localPos.z, localQuat.w, localQuat.x, localQuat.y, localQuat.z);

				parent.addChild(Bone{ (int)i, &parent.data, localPos, localQuat});
			}
		}

		for (Graph<Bone>& child : parent.children)
			addChildRecursive(child, start++);
	}

	void updateWithAnimation(float deltaTime)
	{
		key += deltaTime * animSpeed;

		if (key > GetAnimKeyCount(animName.c_str()))
		{
			key -= (float)GetAnimKeyCount(animName.c_str());
		}

		updateWithAnimationRecurs(*this, deltaTime);
	}

	void updateWithAnimationRecurs(Graph<Bone>& parent, float deltaTime)
	{
		Vector3 newLocalPos;
		Quaternion newLocalRot;

		for (Graph<Bone>& child : parent.children)
		{
			GetAnimLocalBoneTransform(animName.c_str(), child.data.id, (int)key, newLocalPos.x, newLocalPos.y, newLocalPos.z, newLocalRot.w, newLocalRot.x, newLocalRot.y, newLocalRot.z);
			child.data.localDifPos = newLocalPos;
			child.data.localDifRot = newLocalRot;

			updateWithAnimationRecurs(child, deltaTime);
		}
	}

	inline
	const Bone& getRoot() const { return data; }

	void displayConsolGraph()
	{
		displayConsolGraphRecurs(*this);
	}

	void displayConsolGraphRecurs(const Graph<Bone>& parent)
	{
		for (const Graph<Bone>& child : parent.children)
		{
			std::cout << GetSkeletonBoneName(child.data.id) << " is child of " << (parent.data.id == -1 ? "no parent " : GetSkeletonBoneName(parent.data.id)) << std::endl;
			displayConsolGraphRecurs(child);
		}
	}

	void draw()
	{
		drawRecurs(*this);
	}

	void drawRecurs(const Graph<Bone>& parent)
	{
		for (const Graph<Bone>& child : parent.children)
		{
			if (parent.data.id == 0)
			{
				drawRecurs(child);
				break;
			}

			const Vector3 parentglobalPos = parent.data.getGlobalPostion();
			const Vector3 childGlobalPos = child.data.getGlobalPostion();

			DrawLine(parentglobalPos.x, parentglobalPos.y + 100, parentglobalPos.z, childGlobalPos.x, childGlobalPos.y + 100, childGlobalPos.z, 1, 1, 0);
			drawPoint(childGlobalPos);
			drawRecurs(child);
		}
	}

	void drawPoint(Vector3 pos)
	{
		DrawLine(pos.x, pos.y + 100, pos.z, pos.x + 1.5f, pos.y + 100, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y + 100 + 1.5f, pos.z, pos.x + 1.5f, pos.y + 100, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y + 100 + 1.5f , pos.z + 1.5f, pos.x + 1.5f, pos.y + 100 + 1.5f, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y + 100, pos.z + 1.5f, pos.x + 1.5f, pos.y + 100 + 1.5f, pos.z, 1, 0, 0);

	}
};