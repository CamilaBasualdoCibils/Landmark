file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_executable(LandmarkEditor
        ${ENGINE_SOURCES}
)

target_include_directories(LandmarkEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(LandmarkEditor PRIVATE -mavx2 -mavx)
target_link_libraries(LandmarkEditor PRIVATE LandmarkEngine)
