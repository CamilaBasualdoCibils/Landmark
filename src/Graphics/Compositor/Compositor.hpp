#pragma once
#include <pch.h>
#include <misc/Singleton.hpp>
#include "CompositeGroup.hpp"
namespace Graphics
{
    class Compositor : public Singleton<Compositor>
    {
        std::unordered_map<std::string, std::shared_ptr<CompositeGroup>> RootGroups;

        public:
        Compositor();
        [[nodiscard]] std::shared_ptr<CompositeGroup> GetRootGroup(const std::string& Name);
        [[nodiscard]] bool ExistsRootGroup(const std::string& Name);
        [[nodiscard]] std::shared_ptr<CompositeGroup> MakeRootGroup(const CompositeLayerProperties& GroupProperties);
        void DrawDebug();

        void RenderCompositions();
    };
} // namespace Graphics
