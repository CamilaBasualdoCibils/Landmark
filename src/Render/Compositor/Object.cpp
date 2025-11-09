#include "Object.hpp"
#include "Pipeline.hpp"
std::weak_ptr<Render::Composite::Pipeline> Render::Composite::Object::GetPipeline()
{
    auto current = shared_from_this();

    while (!std::dynamic_pointer_cast<Pipeline>(current))
    {
        current = std::dynamic_pointer_cast<Object>(current->GetParent().lock());
    }
    return std::dynamic_pointer_cast<Pipeline>(current);
}

const Render::Composite::Layout& Render::Composite::Object::GetLayout() const
{
    return GetParent().lock()->GetLayout();
}
