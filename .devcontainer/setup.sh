GLEW_DEPS="libxmu-dev libxi-dev libgl-dev"
GLFW_DEPS="libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config"
VULKAN_DEPS="python3 libxcb-keysyms1-dev libxcb-ewmh-dev libxcb-icccm4-dev libxcb-randr0-dev libxcb-util-dev libxcb-xinerama0-dev libwayland-dev wayland-protocols"
VULKAN_SDK="libvulkan-dev"

apt-get update
DEBIAN_FRONTEND=noninteractive apt-get -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" install -y build-essential curl clang clangd zip unzip tar git cmake ${GLEW_DEPS} ${GLFW_DEPS} ${VULKAN_DEPS} ${VULKAN_SDK}
# vcpkg install --x-install-root="libs"
export VCPKG_ROOT=$(whereis -b vcpkg | awk '{print $2}')
