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

	int id;
};

class Skeleton : public Graph<Bone>
{
public : 

	Skeleton()
	{
		data.id = 0;

		int idBoneAccumulator = 0;
		addChildRcursive(*this, idBoneAccumulator);
	}

	void addChildRcursive(Graph<Bone>& parent, int& idBoneAccumulator)
	{
		parent.data.id = idBoneAccumulator;

		do
		{
			idBoneAccumulator++;

			if (GetSkeletonBoneParentIndex(idBoneAccumulator) != parent.data.id)
			{
				addChildRcursive(parent.children.front(), idBoneAccumulator);
			}
			else
			{
				parent.addChild(Bone{ idBoneAccumulator });
			}
		} while (idBoneAccumulator < GetSkeletonBoneCount());
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
			std::cout << (parent.data.id == -1 ? "no parent " : GetSkeletonBoneName(child.data.id)) << " is child of " << GetSkeletonBoneName(parent.data.id) << std::endl;
			displayConsolGraphRecurs(child);
		}
	}

	void draw()
	{
		drawRecurs(Vector3(0,0,0));
	}

	void drawRecurs(Vector3 parentPos)
	{
		Vector3 pos;
		float dump;
		for (const Graph<Bone>& child : children)
		{
			GetSkeletonBoneLocalBindTransform(child.data.id, pos.x, pos.y, pos.z, dump, dump, dump, dump);
			DrawLine(pos.x, pos.y, pos.z, parentPos.x, parentPos.y, parentPos.z, 1, 1, 0);
			drawRecurs(pos);
		}
	}
};