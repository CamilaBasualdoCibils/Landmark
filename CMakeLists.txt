cmake_minimum_required(VERSION 4.0)
project(Landmark)

set(CMAKE_CXX_STANDARD 23)

# Add subprojects
add_subdirectory(LandmarkEngine)
add_subdirectory(LandmarkEditor)
add_subdirectory(GenericLib)
add_subdirectory(FrameGraph)
add_subdirectory(FrameGraphViz)
add_subdirectory(SimpleAppBase)
