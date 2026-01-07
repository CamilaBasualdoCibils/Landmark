//
// Created by camila on 11/10/25.

#ifndef LANDMARK_PASSNODE_HPP
#define LANDMARK_PASSNODE_HPP
#include "FrameGraphPass.hpp"
#include "Libs/Boost.hpp"

#include "FrameGraphResource.hpp"
#include "GraphNode.hpp"
#include <string>
/**
 * A node in the graph that represents a rendering pass
 */
class PassNode final : public GraphNode {
    friend class FrameGraph;

public:
    struct AccessDeclaration {
        FrameGraphResource id;
        uint32_t flags;

        bool operator==(const AccessDeclaration &rhs) const = default;
    };

public:
    [[nodiscard]] bool creates(FrameGraphResource id)const;
    [[nodiscard]] bool writes(FrameGraphResource id)const;
    [[nodiscard]] bool reads(FrameGraphResource id)const;
private:
    PassNode(const std::string_view name, const uint32_t id, std::unique_ptr<IFrameGraphPass> exec);
    FrameGraphResource _read(FrameGraphResource id, uint32_t flags);
    [[nodiscard]] FrameGraphResource _write(FrameGraphResource id, uint32_t flags);
    std::unique_ptr<IFrameGraphPass> exec;

    //the resources that this pass creates
    boost::container::small_vector<FrameGraphResource,10> m_creates;
    //the resources that this pass reads
    boost::container::small_vector<AccessDeclaration,10> m_reads;
    //the resources taht this pass writes
    boost::container::small_vector<AccessDeclaration,10> m_writes;
};


#endif //LANDMARK_PASSNODE_HPP