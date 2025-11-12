file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_library(SimpleAppBase
        ${ENGINE_SOURCES}
)

target_include_directories(SimpleAppBase PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/src

)
target_link_libraries(SimpleAppBase PUBLIC GenericLib)
