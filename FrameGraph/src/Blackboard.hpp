//
// Created by camila on 11/10/25.

#ifndef LANDMARK_BLACKBOARD_HPP
#define LANDMARK_BLACKBOARD_HPP

#include "Utility/Assert.hpp"
#include <any>
#include <typeindex>
#include <unordered_map>
namespace FG {

/**
 * Allows for renderpasses to share resources or link to each other
 */
class Blackboard {
public:
  template <typename T, typename... Args> T &add(Args &&...args) {
    LASSERT(!has<T>(), "Type already exists");
    return _storage[typeid(T)].emplace<T>(T{std::forward<Args>(args)...});
  }

  template <typename T> [[nodiscard]] T &get() {
    LASSERT(has<T>(), "Type does not exist");
    return _storage.at(typeid(T));
  }

  template <typename T> [[nodiscard]] const T &get() const {
    LASSERT(has<T>(), "Type does not exist");
    return _storage.at(typeid(T));
  }

  template <typename T> bool has() const {
    return _storage.contains(typeid(T));
  }

  template <typename T> [[nodiscard]] T *try_get() {
    return has<T>() ? &get<T>() : nullptr;
  }

  template <typename T> [[nodiscard]] const T *try_get() const {
    return has<T>() ? &get<T>() : nullptr;
  }

private:
  std::unordered_map<std::type_index, std::any> _storage;
};
} // namespace FrameGraph

#endif // LANDMARK_BLACKBOARD_HPP
