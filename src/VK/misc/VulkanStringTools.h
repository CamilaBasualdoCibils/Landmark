#include <pch.h>
template <typename T>
uint64_t getVulkanHandle(T const &cppHandle) {
  return uint64_t(static_cast<T::CType>(cppHandle));
}
template <typename T>
std::string getVulkanHandle_Str(T const& cppHandle) {

  std::stringstream ss;
  ss <<"0x"<< std::hex << uint64_t(static_cast<T::CType>(cppHandle));
return ss.str();
}