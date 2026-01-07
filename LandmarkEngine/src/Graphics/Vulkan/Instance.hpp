#pragma once
#include "Libs/Vulkan.hpp"
#include "Utility/Singleton.hpp"
namespace Vulkan {

class Instance : public Singleton<Instance> {

public:
  struct Settings {
    bool MemoryExport;
  };

  explicit Instance(const Settings &);

private:
  Settings settings;
  vk::Instance _instance;
  vk::DebugUtilsMessengerEXT _messenger;
  vk::detail::DispatchLoaderDynamic dldi;

  void CreateInstance();
  void CreateDebugMessenger();
  void ass();
};
} // namespace Vulkan