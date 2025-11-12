file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_executable(FrameGraphViz
        ${ENGINE_SOURCES}
)

target_include_directories(FrameGraphViz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


target_link_libraries(FrameGraphViz PRIVATE FrameGraph SimpleAppBase)
