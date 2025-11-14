cmake_minimum_required(VERSION 3.28)
find_program(VCPKG_EXECUTABLE NAMES vcpkg)

if (NOT VCPKG_EXECUTABLE)
    message(FATAL_ERROR "vcpkg not found in PATH")
endif()

# Get the directory where vcpkg is installed
get_filename_component(VCPKG_ROOT "${VCPKG_EXECUTABLE}" DIRECTORY)

# Set toolchain file
set(CMAKE_TOOLCHAIN_FILE
    "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
    
project(Landmark)


set(CMAKE_CXX_STANDARD 23)

# Add subprojects
add_subdirectory(LandmarkEngine)
add_subdirectory(LandmarkEditor)
add_subdirectory(GenericLib)
add_subdirectory(FrameGraph)
add_subdirectory(FrameGraphViz)
add_subdirectory(SimpleAppBase)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
