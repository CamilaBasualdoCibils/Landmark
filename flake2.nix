{
  description = "Landmark";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };
      in {
        devShells.default = pkgs.mkShell {
          name = "graphics-dev-shell";

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            clang
            clang-tools  # clangd
            vcpkg
            libcxx
            libcxx.dev
          ];

          buildInputs = with pkgs; [
            # OpenGL
            
             cmake
            ninja
            pkg-config
            clang
            gcc
            clang-tools  # clangd
            vcpkg
            glfw
            glew
            mesa
            mesa.drivers
            libGL
            libGLU
            freeglut
            xorg.libXmu
            xorg.libXi
            xorgproto
            xorg.libX11
            xorg.libXext
            xorg.libXrandr
            xorg.libXinerama
            xorg.libXcursor
            xorg.libXdamage
            xorg.libxcb
            wayland
            wayland.dev
            xorg.libXdmcp
            xorg.xcbutil
            xorg.xcbutilimage
            xorg.xcbutilkeysyms
            xorg.xcbutilrenderutil
            xorg.xcbutilwm
            xcb-util-cursor
            wayland-protocols
            wayland-scanner  
            libffi

            # Vulkan
            #vulkan-loader
            #vulkan-headers
            #vulkan-validation-layers
            #vulkan-tools
            #shaderc
            #spirv-tools
            #spirv-headers
            #glslang
          ];
          # Ensures CMake finds Vulkan + OpenGL
          CMAKE_PREFIX_PATH = pkgs.lib.makeSearchPath "lib/cmake" [
            pkgs.vulkan-loader
            pkgs.vulkan-headers
            pkgs.vulkan-validation-layers
            pkgs.glfw
            pkgs.glew
          ];

          # Vulkan SDK env var
          VULKAN_SDK = "${pkgs.vulkan-headers}";

          # Correct env hook
          shellHook = ''
            export VCPKG_ROOT="${pkgs.vcpkg}/share/vcpkg"
            export CMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
          
            echo "Loaded Landmark devShell"
            echo "Using vcpkg: $VCPKG_ROOT"
          '';
        };
      });
}