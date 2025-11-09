#pragma once
#include <misc/Singleton.hpp>
#include <pch.h>
#include "Pipeline.hpp"
namespace Render
{
/**
 * @brief
 *
 */
class Compositor : public Singleton<Compositor>
{

    std::vector<std::weak_ptr<Composite::Pipeline>> Pipelines;

    public:
    Compositor() = default;

    std::shared_ptr<Composite::Pipeline> MakeBasePipeline(std::string_view Name,const Composite::PipelineProperties& properties);

    template <typename T,typename... Args> std::shared_ptr<T> MakePipeline(Args &&...args)
    {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        Pipelines.insert(ptr);
        return ptr;
    }

    void Compose();
};
} // namespace Render
