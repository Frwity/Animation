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
	std::vector<std::vector<Vector3>> animsLocalPos{}; //In function of animation Id and key Id
	std::vector<std::vector<Quaternion>> animsLocalRot{};

	Vector3 actualLocalPos{0.f, 0.f, 0.f};
	Quaternion actualLocalRot{0.f, 0.f, 0.f, 0.f};

	float scale = 1.f;

	Bone(int _id, Bone* _boneParent, const std::vector<std::string>& _anims, const Vector3& localBindPos, const Quaternion& localBindRot)
		: id{_id}, boneParent{_boneParent}
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

	float animSpeed = 10.f;
	int keyCurrentAnim = 0;
	float keyFrameAnim = 0.f;

	Skeleton(const std::vector<std::string>& _animName)
		: Graph<Bone>(0, nullptr, _animName, Vector3(), Quaternion())
	{
		addChildRecursive(*this, data.id, _animName);
	}
	
	void addChildRecursive(Graph<Bone>& parent, int start, const std::vector<std::string>& _animName)
	{
		for (size_t i = start; i < GetSkeletonBoneCount(); ++i)
		{
			if (GetSkeletonBoneParentIndex(i) == parent.data.id)
			{
				Vector3 localPos;
				Quaternion localQuat;
				GetSkeletonBoneLocalBindTransform(i, localPos.x, localPos.y, localPos.z, localQuat.w, localQuat.x, localQuat.y, localQuat.z);

				parent.addChild(Bone{ (int)i, &parent.data, _animName, localPos, localQuat});
			}
		}

		for (Graph<Bone>& child : parent.children)
			addChildRecursive(child, start++, _animName);
	}

	void updateWithAnimation(float deltaTime)
	{
		keyFrameAnim += deltaTime * animSpeed;

		if (keyFrameAnim > data.animsLocalPos[keyCurrentAnim].size())
		{
			keyFrameAnim -= (float)data.animsLocalPos[keyCurrentAnim].size();
		}

		updateWithAnimationRecurs(*this, deltaTime);
	}

	void updateWithAnimationRecurs(Graph<Bone>& parent, float deltaTime)
	{
		for (Graph<Bone>& child : parent.children)
		{
			float t = keyFrameAnim - (int)keyFrameAnim;
		
			std::cout << keyFrameAnim << " " << ((int)keyFrameAnim + 1) % (child.data.animsLocalPos[keyCurrentAnim].size()) << std::endl;

			child.data.actualLocalPos = Vector3::lerp(child.data.animsLocalPos[keyCurrentAnim][keyFrameAnim], child.data.animsLocalPos[keyCurrentAnim][((int)keyFrameAnim + 1) % (child.data.animsLocalPos[keyCurrentAnim].size())], t);
			child.data.actualLocalRot = Quaternion::SLerp(child.data.animsLocalRot[keyCurrentAnim][keyFrameAnim], child.data.animsLocalRot[keyCurrentAnim][((int)keyFrameAnim + 1) % (child.data.animsLocalRot[keyCurrentAnim].size())], t);

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
			//std::cout << " child pos "<< childGlobalPos.x << " " << childGlobalPos.y << " " << childGlobalPos.z << " parent pos " << parentglobalPos.x << " " << parentglobalPos.y << " " << parentglobalPos.z << " ";
			DrawLine(parentglobalPos.x, parentglobalPos.y - 100, parentglobalPos.z, childGlobalPos.x, childGlobalPos.y - 100, childGlobalPos.z, 1, 1, 0);
			drawPoint(childGlobalPos);
			drawRecurs(child);
		}
	}

	void drawPoint(Vector3 pos)
	{
		DrawLine(pos.x, pos.y - 100, pos.z, pos.x + 1.5f, pos.y - 100, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y - 100 + 1.5f, pos.z, pos.x + 1.5f, pos.y - 100, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y - 100 + 1.5f , pos.z + 1.5f, pos.x + 1.5f, pos.y - 100 + 1.5f, pos.z, 1, 0, 0);
		DrawLine(pos.x + 1.5f, pos.y - 100, pos.z + 1.5f, pos.x + 1.5f, pos.y - 100 + 1.5f, pos.z, 1, 0, 0);

	}
};