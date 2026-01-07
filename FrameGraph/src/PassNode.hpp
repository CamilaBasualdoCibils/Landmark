#pragma once
#include "GraphNode.hpp"
#include "Resource.hpp"
#include "Libs/Boost.hpp"
namespace FG {

    class PassNode : public GraphNode
    {


        boost::container::small_vector<Resource,10> m_creates;
    };
}