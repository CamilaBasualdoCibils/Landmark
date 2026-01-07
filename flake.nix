{
  description = "A Nix-flake-based C/C++ development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs {
          inherit system;
        };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell.override { }
          {
            shellHook = /* bash */ ''
              export LD_LIBRARY_PATH=${pkgs.libxkbcommon}/lib:$LD_LIBRARY_PATH

            '';

            packages = with pkgs; [
    cmake
            ninja
            pkg-config
            clang
            clang-tools  # clangd
            vcpkg
            ];

            buildInputs = with pkgs; [
 cmake
            ninja
            pkg-config
            clang
            gcc
            gcc.cc
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
            ];

            nativeBuildInputs = with pkgs; [
                 cmake
            ninja
            pkg-config
            clang
            clang-tools  # clangd
            vcpkg
            libcxx
            libcxx.dev
              # wayland-protocols
              # wayland
              # libxkbcommon
            ];
          };
      });
    };
}