#pragma once
#include "misc/Singleton.hpp"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <pch.h>
#include <type_traits>
namespace Graphics
{

enum class ShaderSourceType : std::underlying_type_t<glslang::EShSource>
{
    eGLSL = (size_t)glslang::EShSourceGlsl,
    eHLSL = (size_t)glslang::EShSourceHlsl,
};
enum class ShaderType : std::underlying_type_t<EShLanguage>
{
    eInvalid,
    eVertex = (size_t)(EShLanguage::EShLangVertex),
    eFragment = (size_t)(EShLanguage::EShLangFragment),
    eGeometry = (size_t)(EShLanguage::EShLangGeometry)
};
enum class ShaderClientType : std::underlying_type_t<glslang::EShClient>
{
    eVulkan_1_0 = (size_t)glslang::EShTargetClientVersion::EShTargetVulkan_1_0,
    eVulkan_1_1 = (size_t)glslang::EShTargetClientVersion::EShTargetVulkan_1_1,
    eVulkan_1_2 = (size_t)glslang::EShTargetClientVersion::EShTargetVulkan_1_2,
    eVulkan_1_3 = (size_t)glslang::EShTargetClientVersion::EShTargetVulkan_1_3,
    eVulkan_1_4 = (size_t)glslang::EShTargetClientVersion::EShTargetVulkan_1_4,
    eOpenGL_450 = (size_t)glslang::EShTargetClientVersion::EShTargetOpenGL_450
};
enum class ShaderSpvVersion : std::underlying_type_t<glslang::EShTargetLanguage>
{
    eSpv_1_0 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_0,
    eSpv_1_1 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_1,
    eSpv_1_2 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_2,
    eSpv_1_3 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_3,
    eSpv_1_4 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_4,
    eSpv_1_5 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_5,
    eSpv_1_6 = (size_t)glslang::EShTargetLanguageVersion::EShTargetSpv_1_6,
};

struct ShaderCompileResult
{
    std::vector<uint32_t> data;
    bool compile_successful = false;
    std::string compile_log;
};
struct ShaderCompileInfo
{
    ShaderType shader_type = ShaderType::eInvalid;
    ShaderSourceType source_type = ShaderSourceType::eGLSL;
    ShaderClientType client_type = ShaderClientType::eVulkan_1_4;
    ShaderSpvVersion spv_version = ShaderSpvVersion::eSpv_1_6;
    std::vector<std::string> Sources;
};
class ShaderCompiler : public Singleton<ShaderCompiler>
{

    static TBuiltInResource GetDefaultResource();

  public:
    ShaderCompiler()
    {
        glslang::InitializeProcess();
    }
    ~ShaderCompiler()
    {
        glslang::FinalizeProcess();
    }
    [[nodiscard]] ShaderCompileResult Compile(const ShaderCompileInfo &info);
};

} // namespace Graphics