#pragma once

#include <vector>
#include <iostream>

#include "Maths/Quaternion.hpp"
#include "Maths/matrix.hpp"
#include "Maths/vector3.hpp"
#include "Graph.h"
#include "Engine.h"

float lerp(float f1, float f2, float t)
{
	return f1 + t * (f2 - f1);
}

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

	float animSpeed = 5.f;
	int idCurrentAnim = 0;
	int idBlendedAnim = -1;
	float keyFrameCurrentAnim = 0.f;
	float keyFrameBlendedAnim = 0.f;
	float blendAnimRatio = 0.f;
	float fadDuration = 1.f;
	float fadCount = 0.f;

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
		float timeScaleCurrentAnim = 1.f;

		if (idBlendedAnim != -1)
		{
			float normalizedTimeScale = data.animsLocalPos[idBlendedAnim].size() / data.animsLocalPos[idCurrentAnim].size();
			float normalizedTimeScale2 = data.animsLocalPos[idCurrentAnim].size() / data.animsLocalPos[idBlendedAnim].size();
			float timeScaleNextAnim = lerp(normalizedTimeScale, 1.f, fadCount / fadDuration);
			timeScaleCurrentAnim = lerp(1.f, normalizedTimeScale2, fadCount / fadDuration);

			keyFrameBlendedAnim += timeScaleNextAnim * deltaTime * animSpeed;

			if (keyFrameBlendedAnim > data.animsLocalPos[idBlendedAnim].size())
			{
				keyFrameBlendedAnim -= (float)data.animsLocalPos[idBlendedAnim].size();
			}			
		}

		keyFrameCurrentAnim += timeScaleCurrentAnim * deltaTime * animSpeed;

		if (keyFrameCurrentAnim > data.animsLocalPos[idCurrentAnim].size())
		{
			keyFrameCurrentAnim -= (float)data.animsLocalPos[idCurrentAnim].size();
		}

		updateWithAnimationRecurs(*this, deltaTime);

		if (idBlendedAnim != -1)
		{
			fadCount += deltaTime;

			if (fadCount >= fadDuration)
			{
				keyFrameCurrentAnim = keyFrameBlendedAnim;
				idCurrentAnim = idBlendedAnim;
				idBlendedAnim = -1;
				fadCount = 0.0f;
				keyFrameBlendedAnim = 0.f;
			}
		}
	}

	void blendAnimWithOther(int newIdAnim, float newFadDuration)
	{
		if (newIdAnim > data.animsLocalPos.size())
			return;

		fadDuration = newFadDuration;
		idBlendedAnim = newIdAnim;
	}

	void updateWithAnimationRecurs(Graph<Bone>& parent, float deltaTime)
	{
		float t = keyFrameCurrentAnim - (int)keyFrameCurrentAnim;
		Vector3 currentPos;
		Vector3 goalPos;

		Quaternion currentRot;
		Quaternion goalRot;
		
		for (Graph<Bone>& child : parent.children)
		{
			int goalNextKeyCurrentAnim = ((int)keyFrameCurrentAnim + 1) % (child.data.animsLocalPos[idCurrentAnim].size());

			if (idBlendedAnim != -1)
			{				
				int goalNextKeyNextAnim = ((int)keyFrameBlendedAnim + 1) % (child.data.animsLocalPos[idBlendedAnim].size());
				

				//std::cout << "idCurrentAnim " << idCurrentAnim << " keyFrameCurrentAnim " << keyFrameCurrentAnim << " idBlendedAnim " << idBlendedAnim <<
				//	" keyFrameBlendedAnim " << keyFrameBlendedAnim << " fadCount / fadDuration " << fadCount / fadDuration << std::endl;

				currentPos = Vector3::lerp(	child.data.animsLocalPos[idCurrentAnim][keyFrameCurrentAnim], 
											child.data.animsLocalPos[idBlendedAnim][keyFrameBlendedAnim], fadCount / fadDuration);

				goalPos = Vector3::lerp(child.data.animsLocalPos[idCurrentAnim][goalNextKeyCurrentAnim],
										child.data.animsLocalPos[idBlendedAnim][goalNextKeyNextAnim], fadCount / fadDuration);

				currentRot = Quaternion::SLerp(child.data.animsLocalRot[idCurrentAnim][keyFrameCurrentAnim], 
												child.data.animsLocalRot[idBlendedAnim][keyFrameBlendedAnim], fadCount / fadDuration);

				goalRot = Quaternion::SLerp(child.data.animsLocalRot[idCurrentAnim][goalNextKeyCurrentAnim],
											child.data.animsLocalRot[idBlendedAnim][goalNextKeyNextAnim], fadCount / fadDuration);

			}
			else
			{
				currentPos = child.data.animsLocalPos[idCurrentAnim][keyFrameCurrentAnim];
				goalPos = child.data.animsLocalPos[idCurrentAnim][goalNextKeyCurrentAnim];
				currentRot = child.data.animsLocalRot[idCurrentAnim][keyFrameCurrentAnim];
				goalRot = child.data.animsLocalRot[idCurrentAnim][goalNextKeyCurrentAnim];
			}

			child.data.actualLocalPos = Vector3::lerp(currentPos, goalPos, t);
			child.data.actualLocalRot = Quaternion::SLerp(currentRot, goalRot, t);

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