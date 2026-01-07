//
// Created by camila on 11/12/25.

#ifndef LANDMARK_RESOURCENODE_HPP
#define LANDMARK_RESOURCENODE_HPP
#include "GraphNode.hpp"


/**
 * A node in the graph that represents a resource
 */
class ResourceNode final : public GraphNode{


private:
    uint32_t id = -1,version = -1;
};


#endif //LANDMARK_RESOURCENODE_HPP