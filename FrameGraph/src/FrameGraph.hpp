#pragma once

#include <functional>
#include <string_view>
#include "Math/Math.hpp"
#include "Resource.hpp"
#include "ResourceEntry.hpp"
#include "ResourceNode.hpp"
#include "PassNode.hpp"
namespace FG
{
    /**
     * @brief Main use of a FrameGraph
     * 
     */
class FrameGraph
{

    public:
    class Builder
    {
        protected:
        Builder(FrameGraph& _fg) :fg(_fg){}
        public:
        template<typename Type,typename Desc = Type::Desc>
        Resource create(const std::string_view name,const Desc& desc)
        {
            const auto ID = fg._create<Type,Desc>(ResourceEntry::ResourceType::eTransient,name,desc,Type{});
            pass_node.
        }

        Resource write(Resource);

        private:
        FrameGraph& fg;
        PassNode& pass_node;

    };
    class PassResources
    {
        public:
        template <typename T>
        [[nodiscard]] T& get(FG::Resource id)
        {
            static T t;
            return t;
        }
    };

    template <typename Data>
    using LambdaSetupCallback = std::function<void(Builder& builder,Data& data)>;
    template <typename Data>
    using LambdaExecuteCallback = std::function<void(const Data& data,PassResources& resources)>;
    template <typename Data>
    const Data& push_pass_lambda(const std::string_view pass_name,LambdaSetupCallback<Data> setup,LambdaExecuteCallback<Data> execute);
    void compile();
    void execute();
    private:
    std::vector<PassNode> pass_nodes;
    std::vector<ResourceEntry> resource_registry;
    std::vector<ResourceNode> resource_nodes;

    template <typename Type,typename Desc = Type::Desc>
    Resource _create(const ResourceEntry::ResourceType type,const std::string_view name, const Desc& desc, Type&& resource)
    {
        const auto ID = static_cast<Resource>(resource_registry.size());
        resource_registry.emplace_back(ResourceEntry{type,ID,desc,std::forward<Type>(resource)});
        //we return a resource node since each time a get is done, 
        // they receive the most recent version. not the original node
        return _create_resource_node(name,ID).get_id();
    }
    ResourceNode& _create_resource_node(const std::string_view name,Resource resource_id,uint32 version = ResourceEntry::k_initial_version)
    {
        const uint32 node_id = resource_nodes.size();
        return resource_nodes.emplace_back(ResourceNode{name,node_id,resource_id,version});
    }
};
};
