//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_FRAMEGRAPH_HPP
#define LANDMARK_FRAMEGRAPH_HPP
#include <string_view>
#include <functional>

#include "FrameGraphPass.hpp"
#include "FrameGraphResource.hpp"
#include "PassNode.hpp"
#include "ResourceEntry.hpp"
#include "ResourceNode.hpp"
#include "Utility/Assert.hpp"
class FrameGraphPassResources;

class FrameGraph {
public:
    static constexpr auto kFlagsIgnored = ~0;

    class Builder final {
    public:
        Builder(FrameGraph &parent_graph, PassNode &pass_node) : parent_graph(parent_graph), pass_node(pass_node) {
        }

        template<typename T>
        [[nodiscard]] FrameGraphResource create(const std::string_view name, const typename T::Desc & desc) {
            const auto id = parent_graph._create<T>(ResourceEntry::Type::eTransient,name,desc,T{});
            return pass_node.m_creates.emplace_back(id);
        }

        FrameGraphResource read(FrameGraphResource id, uint32_t flags = kFlagsIgnored) {
            LASSERT(parent_graph.isValid(id),"Invalid ID");
            return pass_node._read(id,flags);
        }

        [[nodiscard]] FrameGraphResource write(FrameGraphResource id, uint32_t flags = kFlagsIgnored) {
            LASSERT(parent_graph.isValid(id),"Invalid ID");
            if (parent_graph._get_resource_entry(id).isImported()) setSideEffect();

            if (pass_node.creates(id)) {
                return pass_node._write(id, flags);
            } else {
                // Writing to a texture produces a renamed handle.
                // This allows us to catch errors when resources are modified in
                // undefined order (when same resource is written by different passes).
                // Renaming resources enforces a specific execution order of the render
                // passes.
                pass_node._read(id, kFlagsIgnored);
                return pass_node._write(parent_graph._clone(id), flags);
            }
        }

    private:
        FrameGraph &parent_graph;
        PassNode &pass_node;
    };

    struct NoData {
    };

    template<typename Data = NoData, typename Setup, typename Execute>
    const Data &add_callback_pass(const std::string_view name, Setup &&setup, Execute &&execute) {
        auto pass = std::make_unique<FrameGraphPass<Data, Execute> >(std::forward<Execute>(execute));
        auto *passr = pass.get();
        auto &passNode = _create_pass_node(name, std::move(pass));
        Builder builder{*this, passNode};
        std::invoke(setup, builder, passr->data);
        return passr->data;
    }
    [[nodiscard]] bool isValid(FrameGraphResource id) const;
    void compile();

    void execute(void *context = nullptr, void *allocator = nullptr);

private:
    PassNode &_create_pass_node(const std::string_view name, std::unique_ptr<IFrameGraphPass> &&exec) {
        const auto id = static_cast<uint32_t>(m_pass_nodes.size());
        return m_pass_nodes.emplace_back(PassNode{name, id, std::move(exec)});
    }

    template<typename T>
    uint32_t _create(const ResourceEntry::Type type,const std::string_view name,const typename T::Desc &desc,T&&) {

    }
    [[nodiscard]] const ResourceNode& _get_resource_node(FrameGraphResource id) const{
        return const_cast<ResourceNode&>(const_cast<FrameGraph*>(this)->_get_resource_node(id));
    }
    [[nodiscard]] const ResourceEntry& _get_resource_entry(FrameGraphResource id) const
    {
        return const_cast<ResourceEntry&>(const_cast<FrameGraph*>(this)->_get_resource_entry(id));
    }
    [[nodiscard]] const ResourceEntry& _get_resource_entry(const ResourceNode& id) const
    {
        return const_cast<ResourceEntry&>(const_cast<FrameGraph*>(this)->_get_resource_entry(id));
    }
    //All resource IDs
    boost::container::small_vector<ResourceEntry, 25> resource_registry;
    //All resource nodes
    boost::container::small_vector<ResourceNode, 25> resources_nodes;
    // boost::container::small_vector<ResourceEntry, 25> resources;
    //All pass nodes
    boost::container::small_vector<PassNode, 10> m_pass_nodes;
};

class FrameGraphPassResources {
public:
    template<typename T>
    [[nodiscard]] T &get(FrameGraphResource id) {
        LASSERT(pass_node_.reads(id) || pass_node_.writes(id) || pass_node_.creates(id),
                "Cannot get resources not associated");
        throw "Not implemented";
    }

    template<typename T>
    [[nodiscard]] const T::Desc &get_descriptor(FrameGraphResource id) const {
        throw "Not implemented";
    }

private:
    FrameGraph &parent_graph;
    const PassNode pass_node_;
};

#endif //LANDMARK_FRAMEGRAPH_HPP
