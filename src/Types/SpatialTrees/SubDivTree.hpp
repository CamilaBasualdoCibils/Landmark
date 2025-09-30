#pragma once
#include "MortonOrdering.hpp"
#include <Types/AABB.hpp>
#include <pch.h>
template <uint8 Dim, uint32 TreeDepth, typename LeafData = void, typename BranchData = void> class SubDivTree
{

    using MortonIndex = uint32;
    using VoxelPosition = uint32;
    constexpr static inline uint32 MAX_DEPTH = 32; // given 2^32 is the max a uint32 coord can store
    struct Node
    {
        std::bitset<8> ChildrenSet = 0;
        // std::array<MortonIndex, 8> children = {0};
        LeafData data;
        bool HasChildAtIndex(uint32 index) const
        {
            return ChildrenSet.test(index);
        }
        uint8 GetChildCount() const
        {
            return ChildrenSet.count();
        }
    };
    struct custom_hash_simple
    {
        auto operator()(MortonIndex const &x) const noexcept -> uint64_t
        {
            return x;
        }
    };
    using NodesContainer = ankerl::unordered_dense::map<MortonIndex, Node>;
    //  using NodesContainer = ankerl::unordered_dense::segmented_map<MortonIndex,Node,custom_hash_simple>;
    //using NodesContainer = std::unordered_map<MortonIndex, Node>;
    // using NodesContainer = std::vector<Node>;
    std::array<NodesContainer, TreeDepth + 1> Levels;

    uint32 SignToChildIndex(ivec3 sign)
    {
        return ((sign.x >= 0) << 2) | ((sign.y >= 0) << 1) | (sign.z >= 0);
    }
    vec<3, int8> ChildIndexToSign(uint32 index)
    {
        ivec3 sign;
        for (int i = 0; i < 3; i++)
            sign[i] = ((index >> (2 - i)) & 0b1) ? 1 : -1;
        return sign;
    }

    MortonIndex IndexFromChildPos(uvec3 ChildPos, uint32 ChildDepth)
    {
        return Morton::SIMD::MortonEncodeBounded<3>(ChildPos / SizeOfNodeAtDepth(ChildDepth),
                                                    SubDivTreeResolution(ChildDepth));
    }

  public:
    SubDivTree();

    /// @brief Gets the deepest node that occupies that position
    /// @param Position
    /// @param Level Specify a specific level to look for
    /// @return
    Node &AtPositionNode(uvec3 Position, std::optional<uint32> Level);
    /// @brief Get the Leaf node at a given global position of last depth
    /// @param Position
    /// @return
    Node &AtPositionLeaf(uvec3 Position);
    /// @brief Gets the node that exists at a specified level and MortonIndex
    /// @param level
    /// @param index
    /// @return
    Node &AtMortonIndex(uint32 level, MortonIndex index);

    Node &InsertLeaf(uvec3 GlobalPosition);
    void InsertLeafs(const std::vector<uvec3> &positions);

    void Clear()
    {
        for (int i = 0; i < TreeDepth; i++)
        {
            Levels[i].clear();
        }
    }
    constexpr uint32 SizeOfNodeAtDepth(uint32 depth) const
    {
        return uint32(1) << (TreeDepth - depth);
    };
    constexpr uint32 SubDivTreeResolution(uint32 AtDepth = TreeDepth) const
    {
        return uint32(1) << AtDepth;
    }
    constexpr size_t AbsMaxMemoryUse() const
    {
        size_t size = 0;
        for (int i = 0; i < TreeDepth; i++)
        {
            const uint32 res = SubDivTreeResolution(i);
            size += res * res * res * sizeof(Node);
        }
        return size;
    }
    size_t CurrentMemoryUsage() const
    {
        size_t size = 0;
        for (int i = 0; i < TreeDepth; i++)
        {
            size += sizeof(Node) * Levels[i].size();
        }
        return size;
    }
    constexpr uint32 GetDepth() const
    {
        return TreeDepth;
    }

    struct NodeData
    {
        MortonIndex mortonCode;
        uint32 Depth;
        uint32 NodeSizeXYZ;
        uvec3 NodePos;
        bool isLeaf;
        std::variant<LeafData *, BranchData *> data;

        template <typename J> J *GetData() const
        {
            LASSERT(std::holds_alternative<J *>(data), "Not realy type");
            return std::get<J *>(data);
        }
        LeafData *GetLeafData() const
        {
            return GetData<LeafData>();
        }
        BranchData *GetBranchData() const
        {
            return GetData<BranchData>();
        }
    };

    void ForEach(std::function<void(const NodeData &)> Func);
};

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline SubDivTree<TreeDepth, LeafData, BranchData>::SubDivTree()
{
    static_assert(TreeDepth <= MAX_DEPTH, "Exceeds maximum depth");
    // Levels[0].resize(1);
}

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline SubDivTree<TreeDepth, LeafData, BranchData>::Node &SubDivTree<TreeDepth, LeafData, BranchData>::AtPositionLeaf(
    uvec3 Position)
{
    uvec3 NodePosition = uvec3(0);
    for (int i = 0; i < TreeDepth; i++)
    {
        const uvec3 NodeSize = SizeOfNodeAtDepth(i);
    }
}

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline SubDivTree<TreeDepth, LeafData, BranchData>::Node &SubDivTree<TreeDepth, LeafData, BranchData>::AtMortonIndex(
    uint32 level, MortonIndex index)
{
    return Levels[level][index];
}

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline SubDivTree<TreeDepth, LeafData, BranchData>::Node &SubDivTree<TreeDepth, LeafData, BranchData>::InsertLeaf(
    uvec3 GlobalPosition)
{
    LASSERT(glm::all(glm::greaterThanEqual(GlobalPosition, uvec3(0))) &&
                glm::all(glm::lessThan(GlobalPosition, uvec3(SubDivTreeResolution()))),
            "Invalid Position");
    Node *currentNode = &Levels[0][0];
    uvec3 Position = {0, 0, 0};
    for (int idepth = 0; idepth < TreeDepth; idepth++)
    {
        const uint32 NodeSizeXYZ = SizeOfNodeAtDepth(idepth);
        const AABB3i nodeAABB(Position, Position + uvec3(NodeSizeXYZ));
        const ivec3 childSign = nodeAABB.SignFromCenter(GlobalPosition);
        const uint32 ChildIndex = SignToChildIndex(childSign);
        uvec3 PositionOfChild = Position + uvec3(glm::min(childSign + ivec3(1), ivec3(1))) * uint32(NodeSizeXYZ / 2);
        const MortonIndex EncodedChildPosition = IndexFromChildPos(PositionOfChild, idepth + 1);
        auto &ChildBuffer = Levels[idepth + 1];
        currentNode->ChildrenSet.set(ChildIndex);
        if (!ChildBuffer.contains(EncodedChildPosition))
        {
            ChildBuffer.insert(std::make_pair(EncodedChildPosition, Node()));
        }
        Node *Child = &ChildBuffer.at(EncodedChildPosition);
        currentNode = Child;
        Position += glm::min(childSign + ivec3(1), ivec3(1)) * int32(NodeSizeXYZ / 2);
    }

    return *currentNode;
}

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline void SubDivTree<TreeDepth, LeafData, BranchData>::InsertLeafs(const std::vector<uvec3> &positions)
{
    static std::vector<std::pair<uvec3, MortonIndex>> posMorton;
    // std::pair<uvec3, uint32> posMorton[positions.size()];
    posMorton.clear();
    posMorton.reserve(positions.size());
    int i = 0;
    for (auto &pos : positions)
    {
        MortonIndex idx = Morton::SIMD::MortonEncode<3>(pos); // implement MortonEncode(x,y,z)
        // posMorton[i] = {pos, idx};
        posMorton.push_back({pos, idx});
        i++;
    }
    std::sort(posMorton.begin(), posMorton.end(), [](const auto &a, const auto &b) { return a.second < b.second; });

    for (const auto &[pos, morton] : posMorton)
    {
        InsertLeaf(pos);
    }
}

template <uint32 TreeDepth, typename LeafData, typename BranchData>
inline void SubDivTree<TreeDepth, LeafData, BranchData>::ForEach(std::function<void(const NodeData &)> func)
{
    struct NodeInfo
    {
        uvec3 NodePos;
        MortonIndex mortonIndex;
        uint32 Depth;
        Node *node;
        bool isLeaf = false;
    };
    std::stack<NodeInfo> stack;
    uint64 NodesProcessed = 0;
    stack.push(NodeInfo{.NodePos = {0, 0, 0}, .mortonIndex = 0, .Depth = 0, .node = &Levels.at(0).at(0)});
    while (!stack.empty())
    {
        NodeInfo Current = stack.top();
        Node *node = Current.node;
        stack.pop();
        NodesProcessed++;
        NodeData iterData;
        iterData.Depth = Current.Depth;
        iterData.isLeaf = Current.isLeaf;
        iterData.mortonCode = Current.mortonIndex;
        iterData.NodePos = Current.NodePos;
        iterData.data = &node->data;
        func(iterData);
        if (Current.isLeaf)
        {
            continue;
        }
        for (uint8 i = 0; i < 8; i++)
        {
            if (node->HasChildAtIndex(i))
            {
                NodeInfo ChildInfo;
                ChildInfo.Depth = Current.Depth + 1;
                vec<3, int8> ChildSignVector = glm::max(ChildIndexToSign(i), vec<3, int8>(0));

                ChildInfo.NodePos = Current.NodePos + (SizeOfNodeAtDepth(Current.Depth) / 2) * (uvec3)ChildSignVector;
                ChildInfo.mortonIndex = IndexFromChildPos(ChildInfo.NodePos, ChildInfo.Depth);
                //std::cerr << "Fetching node at depth " << ChildInfo.Depth << " index: " << ChildInfo.mortonIndex<< " Position " << to_string(ChildInfo.NodePos) << std::endl;
                ChildInfo.node = &Levels.at(ChildInfo.Depth).at(ChildInfo.mortonIndex);
                ChildInfo.isLeaf = ChildInfo.Depth == TreeDepth;
                stack.push(ChildInfo);
            }
        }
    }
}
