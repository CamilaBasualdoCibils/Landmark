#pragma once
#include "GraphNode.hpp"
#include "Math/Math.hpp"
#include <memory>
#include <string_view>

namespace FG {
class PassNode;
class FrameGraph;
class ResourceNode final: public GraphNode {
    friend class FrameGraph;
public:
  ResourceNode(const std::string_view name, uint32 _node_id,
               uint32 _resource_id, uint32 _version)
      : GraphNode(name, _node_id), resource_id(_resource_id),
        version(_version) {}


    [[nodiscard]] auto get_id() const {return resource_id;}

private:
  uint32 resource_id, version;
  std::shared_ptr<PassNode> producer, last_user;
};

} // namespace FG