//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_GRAPHNODE_HPP
#define LANDMARK_GRAPHNODE_HPP
#include <string>
#include <cstdint>
#include <string_view>
namespace FG {

class FrameGraph;
/**
 * A generic Node in the graph
 */
class GraphNode {
  friend class FrameGraph;

public:
  GraphNode() = delete;
  GraphNode(const GraphNode &) = delete;
  GraphNode(GraphNode &&) noexcept = default;
  virtual ~GraphNode() = default;

  GraphNode &operator=(const GraphNode &) = delete;

protected:
  GraphNode(const std::string_view name, uint32_t id) : name(name), id(id) {}

private:
  std::string name;
  const uint32_t id;
  int32_t refCount = 0;
};

} // namespace FrameGraph
#endif // LANDMARK_GRAPHNODE_HPP