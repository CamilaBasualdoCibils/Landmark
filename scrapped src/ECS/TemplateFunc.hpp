#include <ECS/Base/ComponentData.hpp>
template <typename T>
using Enable_if_component = std::enable_if_t<std::is_base_of_v<IComponentData, T>>;