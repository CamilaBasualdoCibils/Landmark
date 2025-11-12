file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_library(GenericLib
        ${ENGINE_SOURCES}
)
# Specify public include path with a prefix folder "A"
target_include_directories(GenericLib PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/src

)

# ==============================
# Graphics & Rendering Libraries
# ==============================

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(Stb REQUIRED)

target_link_libraries(GenericLib PUBLIC
        ${OPENGL_LIBRARIES}
        ${EGL_LIB}
        GLEW::GLEW
        glfw
        glm::glm
        glslang::glslang
        glslang::glslang-default-resource-limits
        glslang::SPIRV
        glslang::SPVRemapper
        Vulkan::Headers
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
)

target_include_directories(GenericLib PUBLIC
        ${Stb_INCLUDE_DIR}
)

# ==============================
# UI Libraries
# ==============================

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(implot3d CONFIG REQUIRED)

target_link_libraries(GenericLib PUBLIC
        imgui::imgui
        implot::implot
        implot3d::implot3d
)

# ==============================
# Utility & Data Libraries
# ==============================

find_package(nlohmann_json CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

target_link_libraries(GenericLib PUBLIC
        nlohmann_json::nlohmann_json
        EnTT::EnTT
)

# ==============================
# Boost Components
# ==============================

find_package(boost_container CONFIG REQUIRED)
find_package(boost_describe CONFIG REQUIRED)
find_package(boost_type_traits CONFIG REQUIRED)
find_package(boost_multi_index CONFIG REQUIRED)
find_package(boost_uuid CONFIG REQUIRED)

target_link_libraries(GenericLib PUBLIC
        Boost::container
        Boost::describe
        Boost::type_traits
        Boost::multi_index
        Boost::uuid
)

# ==============================
# SPIR-V Tools
# ==============================

find_package(SPIRV-Tools CONFIG REQUIRED)
target_link_libraries(GenericLib PUBLIC SPIRV-Tools-static)
