#include <pch.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <GenericEnums.h>

_CREATE_ENUM_(ShaderSourceType, glslang::EShSource){
    GLSL = int_cast(glslang::EShSourceGlsl),
    HLSL = int_cast(glslang::EShSourceHlsl),

};
_CREATE_ENUM_(ShaderType, EShLanguage){
    INVALID,
    VERTEX = int_cast(EShLanguage::EShLangVertex),
    FRAGMENT = int_cast(EShLanguage::EShLangFragment),
    GEOMETRY = int_cast(EShLanguage::EShLangGeometry)

};
_CREATE_ENUM_(ShaderClientType, glslang::EShClient){
    VULKAN_1_0 = int_cast(glslang::EShTargetClientVersion::EShTargetVulkan_1_0),
    VULKAN_1_1 = int_cast(glslang::EShTargetClientVersion::EShTargetVulkan_1_1),
    VULKAN_1_2= int_cast(glslang::EShTargetClientVersion::EShTargetVulkan_1_2),
    VULKAN_1_3= int_cast(glslang::EShTargetClientVersion::EShTargetVulkan_1_3),
    VULKAN_1_4= int_cast(glslang::EShTargetClientVersion::EShTargetVulkan_1_4),
    OPENGL_450 = int_cast(glslang::EShTargetClientVersion::EShTargetOpenGL_450)
};
_CREATE_ENUM_(ShaderSpvVersion, glslang::EShTargetLanguage){
    Spv_1_0= glslang::EShTargetLanguageVersion::EShTargetSpv_1_0,
    Spv_1_1= glslang::EShTargetLanguageVersion::EShTargetSpv_1_1,
    Spv_1_2= glslang::EShTargetLanguageVersion::EShTargetSpv_1_2,
    Spv_1_3= glslang::EShTargetLanguageVersion::EShTargetSpv_1_3,
    Spv_1_4= glslang::EShTargetLanguageVersion::EShTargetSpv_1_4,
    Spv_1_5= glslang::EShTargetLanguageVersion::EShTargetSpv_1_5,
    Spv_1_6 = glslang::EShTargetLanguageVersion::EShTargetSpv_1_6,
};
class GLSLANG
{
    static inline std::shared_ptr<GLSLANG> static_glsl_lang;

public:
    static decltype(static_glsl_lang) GetGlslLang()
    {
        if (!static_glsl_lang)
            static_glsl_lang = std::make_shared<GLSLANG>();
        return static_glsl_lang;
    }
    GLSLANG()
    {
        glslang::InitializeProcess();
    }
    ~GLSLANG()
    {
        glslang::FinalizeProcess();
    }
};
struct ShaderCompileResult
{
    std::vector<uint32_t> data;
    bool compile_successful = false;
    std::string compile_log;
};
class Shader
{
    std::shared_ptr<GLSLANG> glsl_lang = GLSLANG::GetGlslLang();

    std::vector<std::string> sources;
    ShaderType shader_type = ShaderType::VERTEX;
    ShaderSourceType source_type = ShaderSourceType::GLSL;
    ShaderClientType client_type = ShaderClientType::VULKAN_1_4;
    ShaderSpvVersion spv_version = ShaderSpvVersion::Spv_1_6;
    static TBuiltInResource GetDefaultResource();
public:
    Shader();
    Shader &SetType(ShaderType type);
    Shader &SetSource(const std::vector<std::string> &_sources);
    Shader &SetSourceType(ShaderSourceType _type);
    Shader &SetClient(ShaderClientType _client);
    Shader &SetTargetSpv(ShaderSpvVersion version);
        ShaderCompileResult Compile();
};
