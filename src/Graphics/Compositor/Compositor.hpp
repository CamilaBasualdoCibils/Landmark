#pragma once
#include <pch.h>
#include <misc/Singleton.hpp>
#include "CompositeGroup.hpp"
namespace Graphics
{
    class Compositor : public Singleton<Compositor>
    {
        std::vector<std::pair<uint32_t,std::shared_ptr<CompositeGroup>>> RootGroups;

        public:
        Compositor();
        ~Compositor()
        {
        std::cerr << "Compositor exit\n";

        }
        std::shared_ptr<CompositeGroup> MakeRootGroup(const CompositeGroupProperties& GroupProperties,int32_t Priority = 0);
        void DrawDebug();

        void RenderCompositions();
    };
} // namespace Graphics
