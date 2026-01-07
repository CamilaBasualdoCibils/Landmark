//
// Created by camila on 11/10/25.
//

#include "FrameGraph.hpp"

bool FrameGraph::isValid(FrameGraphResource id) const {
    const auto &node = _getResourceNode(id);
    return node.getVersion() == _getResourceEntry(node).getVersion();
}

void FrameGraph::compile() {
}

void FrameGraph::execute(void *context, void *allocator) {
}
