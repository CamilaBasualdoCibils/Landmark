#pragma once

#include <limits>
#include <vector>
#include <list>
#include <functional>
#include <queue>
#include "lobby.h"

using namespace glm;
//using namespace std;



template<typename Data, unsigned int DepthWRoot,typename BranchData = const void*>
class Octree
{
	static constexpr int intPower(int value, unsigned power) {
		return power == 0 ? 1 : value * intPower(value, power - 1);
	}
	const static uint64_t MaxNodesPerLayer = intPower(8, DepthWRoot - 1);

	typedef std::conditional_t < MaxNodesPerLayer <= UINT8_MAX, uint8_t
		, std::conditional_t<MaxNodesPerLayer <= UINT16_MAX, uint16_t,
		std::conditional_t<MaxNodesPerLayer <= UINT32_MAX, uint32_t,
		std::conditional_t<MaxNodesPerLayer<= UINT64_MAX,uint64_t,const void*>>>>
	::type IndexDataType;

	const static IndexDataType NullEqu = IndexDataType(~0);
	const static uint64_t EdgeSize = intPower(2, DepthWRoot - 1);


	template<typename BranchData>
	struct Branch
	{
		BranchData data;
		IndexDataType Children[8] = { NullEqu };
		IndexDataType Neighbours[6] = { NullEqu };
		IndexDataType Parent = NullEqu;

	};
	template<>
	struct Branch<void>
	{
		IndexDataType Children[8] = { NullEqu };
		IndexDataType Neighbours[6] = { NullEqu };
		IndexDataType Parent = NullEqu;
	};
	template<typename Data>
	struct Leaf
	{
		Data data;
		IndexDataType neighbours[6] = { NullEqu };
		IndexDataType Parent = NullEqu;
	};

	typedef Branch<BranchData> branch;
	typedef Leaf<Data> leaf;

	std::lobby<leaf> _Leaves = {};
	std::lobby<branch> _branches[DepthWRoot - 1] = {{}};



private:

	static char EncodeAxis(const ivec3 axis)
	{
		char Code = 0;
		for (int i = 0; i < 3; i++)
		{
			int v = axis[i];

			if (v != 0)
			{
				Code += 2 * i;
				Code += v > 0;
				return Code;
			}
		}
		return Code;
	}
	static ivec3 DecodeAxis(const char code)
	{
		ivec3 DirAttempt = { 0,0,0 };
		DirAttempt[code / 2] = (code % 2) * 2 - 1;
		return DirAttempt;
	}

	//for children and such. since there are 8
	static char EncodePos(const ivec3 Dir)
	{
		char v = 0;
		v |= (Dir.x > 0) << 2;
		v |= (Dir.y > 0) << 1;
		v |= (Dir.z > 0) << 0;
		return v;
	}
	static bvec3 DecodePos(const char dir)
	{
		bvec3 Decoded = { 0,0,0 };
		Decoded.x = dir & 0b100;
		Decoded.y = dir & 0b010;
		Decoded.z = dir & 0b001;
		return Decoded;

	}
	bool WithinParentRange(ivec3 pos)
	{
		return pos.x >= 0 && pos.x < 2 &&
			pos.y >= 0 && pos.y < 2 &&
			pos.z >= 0 && pos.z < 2;
	}
	void GetNeighbours(const branch* node, branch* neighbours[6])
	{
		branch* Parent = node->Parent;
		char NodesParentCode = -1;
		if (Parent != nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				if (Parent->children[i] == node)
				{
					NodesParentCode = i;
					break;
				}
			}
		}



		bvec3 NodesParentPos = DecodePos(NodesParentCode);



		for (int i = 0; i < 6; i++)
		{

			ivec3 DirAttempt = DecodeAxis(i);

			ivec3 NeighbourPos = (ivec3)NodesParentPos + DirAttempt;

			if (WithinParentRange(NeighbourPos))
			{
				neighbours[i] = Parent->children[EncodePos(NeighbourPos)];
			}
			else
			{

			}
		}
	}
public:
	Octree()
	{

	}

	void Set(uvec3 pos,Data data)
	{
		assert(pos.x >= 0 && pos.x < EdgeSize
			&& pos.y >= 0 && pos.y < EdgeSize
			&& pos.z >= 0 && pos.z < EdgeSize, 
			"Attempted to set Voxel outside of the bounds of the octree");

		const int middleOfTreeBase = EdgeSize / 2;

		uvec3 currentMiddle = { middleOfTreeBase,middleOfTreeBase,middleOfTreeBase };
		int currentJump = middleOfTreeBase / 2;


		branch* currentNode = &_branches[0][0];
		


		

	}


};


