//
// Created by camila on 11/10/25.
//

#include "PassNode.hpp"

#include "Utility/Assert.hpp"

PassNode::PassNode(const std::string_view name, const uint32_t id,
                   std::unique_ptr<IFrameGraphPass> exec) : GraphNode(name, id), exec(std::move(exec)) {
}

FrameGraphResource PassNode::_read(FrameGraphResource id, uint32_t flags) {
    LASSERT(!creates(id) && !writes(id),"Cannot read from a resource you create or write");
    return  (std::find(m_reads.cbegin(), m_reads.cend(),AccessDeclaration{id,flags})!= m_reads.cend())
               ? id
               : m_reads.emplace_back(AccessDeclaration{id, flags}).id;
}

FrameGraphResource PassNode::_write(FrameGraphResource id, uint32_t flags) {
}

bool PassNode::creates(FrameGraphResource id) const {
    return std::find(m_creates.cbegin(), m_creates.cend(), id) != m_creates.end();
}

bool PassNode::writes(FrameGraphResource id) const {
    return std::find_if(m_writes.cbegin(), m_writes.cend(), [id](const auto &acc) { return acc.id == id; }) != m_writes.
           cend();
}

bool PassNode::reads(FrameGraphResource id) const {
    return std::find_if(m_reads.cbegin(), m_reads.cend(), [id](const auto &acc) { return acc.id == id; }) != m_reads.
           cend();
}
