#pragma once

#include "Math/Math.hpp"
#include <type_traits>
#include <utility>

namespace FG {
template <typename, typename, typename = std::void_t<>>
struct has_pre_read : std::false_type {};

template <typename Type, typename Desc>
struct has_pre_read<
    Type,
    Desc,
    std::void_t<decltype(std::declval<Type>().pre_read(
        std::declval<Desc>(),                      // Desc argument
        std::declval<unsigned>(),                  // unsigned argument
        static_cast<void*>(nullptr)                // void* argument
    ))>
> : std::true_type {};

template <typename Type, typename Desc>
inline constexpr bool has_pre_read_v = has_pre_read<Type, Desc>::value;

class ResourceEntry {
public:
  enum class ResourceType { eTransient, eImported };
  static constexpr uint32 k_initial_version = 1u;
  template <typename Type, typename Desc = Type::Desc>
  ResourceEntry(const ResourceType _resource_type, uint32 id, const Desc &,
                Type &&)
      : resource_type(_resource_type), resource_id(id) {}

private:
  struct IResource {
    virtual ~IResource() = default;
    virtual void create(void *) = 0;
    virtual void destroy(void *) = 0;
    virtual void pre_read(uint32 flags, void *) = 0;
    virtual void pre_write(uint32 flags, void *) = 0;
    virtual std::string to_string() const = 0;
  };
  template <typename Type, typename Desc = Type::Desc>
  struct ResourceWrap : IResource {
    ResourceWrap(const Desc &_descriptor, Type &&_t)
        : descriptor(_descriptor), resource(std::move(_t)) {}

    void create(void *allocator) override {}
    void destroy(void *allocator) override {}
    void pre_read(uint32 flags, void *context) {
      if constexpr (has_pre_read_v<Type,Desc>)
      {

      }
    }

    const Desc descriptor;
    Type resource;
  };
  const ResourceType resource_type;
  const uint32 resource_id, version;
};
} // namespace FG
