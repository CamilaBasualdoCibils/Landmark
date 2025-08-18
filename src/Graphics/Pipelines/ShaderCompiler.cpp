#include "ShaderCompiler.hpp"

Graphics::ShaderCompileResult Graphics::ShaderCompiler::Compile(const Graphics::ShaderCompileInfo &Info)
{
    ShaderCompileResult result;

    std::vector<const char *> strings;
    strings.push_back(Info.Source.c_str());

    glslang::EShClient client;
    std::string Preamble;
    switch (Info.client_type)
    {
    case ShaderClientType::eVulkan_1_0:
    case ShaderClientType::eVulkan_1_1:
    case ShaderClientType::eVulkan_1_2:
    case ShaderClientType::eVulkan_1_3:
    case ShaderClientType::eVulkan_1_4:
        client = glslang::EShClient::EShClientVulkan;
        Preamble += "#define __VULKAN__\n";
        break;
    case ShaderClientType::eOpenGL_450:
        client = glslang::EShClient::EShClientOpenGL;
        Preamble += "#define __OPENGL__\n";
        break;

    default:
        break;
    }
    glslang::SpvOptions options;
    options.stripDebugInfo = false;
    options.generateDebugInfo = true;
    #if _DEBUG
    options.disableOptimizer = true;
    #endif
    glslang::TShader tshader((EShLanguage)Info.shader_type);
    tshader.setDebugInfo(true);
    tshader.setStrings(strings.data(), strings.size());
    tshader.setEnvInput((glslang::EShSource)Info.source_type, (EShLanguage)Info.shader_type, client,
                        (glslang::EShTargetClientVersion)Info.client_type);
    tshader.setEnvClient(client, (glslang::EShTargetClientVersion)Info.client_type);
    tshader.setEnvTarget(glslang::EShTargetSpv, (glslang::EShTargetLanguageVersion)Info.spv_version);
    tshader.setPreamble(Preamble.c_str());
    // tshader.setSourceEntryPoint("main");
    const TBuiltInResource resource = GetDefaultResource();
    bool shader_compile = tshader.parse(&resource, 450, true, EShMsgDefault);

    result.compile_log = tshader.getInfoLog();
    if (!shader_compile)
    {
        return result;
    }
    glslang::TProgram program;
    program.addShader(&tshader);
    const bool program_link = program.link(EShMsgDefault);

    result.compile_successful = shader_compile && program_link;
    result.compile_log += program.getInfoLog();
    if (!program_link)
    {
        return result;
    }
    std::vector<uint32_t> TrueData;
    glslang::GlslangToSpv(*program.getIntermediate((EShLanguage)Info.shader_type), TrueData,&options);
    result.data.resize(TrueData.size() * sizeof(TrueData[0]));
    std::memcpy(result.data.data(), TrueData.data(), result.data.size());

    return result;
}

Graphics::InteropShaderCompileResult Graphics::ShaderCompiler::CompileInterop(const InteropShaderCompileInfo &info)
{
    InteropShaderCompileResult result;

    ShaderCompileInfo GLCompile{.shader_type = info.shader_type,
                                .source_type = info.source_type,
                                .client_type = ShaderClientType::eOpenGL_450,
                                .spv_version = info.spv_version,
                                .Source = info.Source};

    ShaderCompileInfo VKCompile{.shader_type = info.shader_type,
                                .source_type = info.source_type,
                                .client_type = ShaderClientType::eVulkan_1_4,
                                .spv_version = info.spv_version,
                                .Source = info.Source};
    result.GLResult = Compile(GLCompile);
    result.VKResult = Compile(VKCompile);
    result.CompileSuccessful = result.GLResult.compile_successful && result.VKResult.compile_successful;
    result.CompileLog = "VK:\n" + result.VKResult.compile_log + "\nGL:\n" + result.GLResult.compile_log + '\n';
    return result;
}

TBuiltInResource Graphics::ShaderCompiler::GetDefaultResource()
{
    TBuiltInResource resources = {};
    resources.maxLights = 32;
    resources.maxClipPlanes = 6;
    resources.maxTextureUnits = 32;
    resources.maxTextureCoords = 32;
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingFloats = 64;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 80;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 32;
    resources.maxVertexUniformVectors = 128;
    resources.maxVaryingVectors = 8;
    resources.maxFragmentUniformVectors = 16;
    resources.maxVertexOutputVectors = 16;
    resources.maxFragmentInputVectors = 15;
    resources.minProgramTexelOffset = -8;
    resources.maxProgramTexelOffset = 7;
    resources.maxClipDistances = 8;
    resources.maxComputeWorkGroupCountX = 65535;
    resources.maxComputeWorkGroupCountY = 65535;
    resources.maxComputeWorkGroupCountZ = 65535;
    resources.maxComputeWorkGroupSizeX = 1024;
    resources.maxComputeWorkGroupSizeY = 1024;
    resources.maxComputeWorkGroupSizeZ = 64;
    resources.maxComputeUniformComponents = 1024;
    resources.maxComputeTextureImageUnits = 16;
    resources.maxComputeImageUniforms = 8;
    resources.maxComputeAtomicCounters = 8;
    resources.maxComputeAtomicCounterBuffers = 1;
    resources.maxVaryingComponents = 60;
    resources.maxVertexOutputComponents = 64;
    resources.maxGeometryInputComponents = 64;
    resources.maxGeometryOutputComponents = 128;
    resources.maxFragmentInputComponents = 128;
    resources.maxImageUnits = 8;
    resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    resources.maxCombinedShaderOutputResources = 8;
    resources.maxImageSamples = 0;
    resources.maxVertexImageUniforms = 0;
    resources.maxTessControlImageUniforms = 0;
    resources.maxTessEvaluationImageUniforms = 0;
    resources.maxGeometryImageUniforms = 0;
    resources.maxFragmentImageUniforms = 8;
    resources.maxCombinedImageUniforms = 8;
    resources.maxGeometryTextureImageUnits = 16;
    resources.maxGeometryOutputVertices = 256;
    resources.maxGeometryTotalOutputComponents = 1024;
    resources.maxGeometryUniformComponents = 1024;
    resources.maxGeometryVaryingComponents = 64;
    resources.maxTessControlInputComponents = 128;
    resources.maxTessControlOutputComponents = 128;
    resources.maxTessControlTextureImageUnits = 16;
    resources.maxTessControlUniformComponents = 1024;
    resources.maxTessControlTotalOutputComponents = 4096;
    resources.maxTessEvaluationInputComponents = 128;
    resources.maxTessEvaluationOutputComponents = 128;
    resources.maxTessEvaluationTextureImageUnits = 16;
    resources.maxTessEvaluationUniformComponents = 1024;
    resources.maxTessPatchComponents = 120;
    resources.maxPatchVertices = 32;
    resources.maxTessGenLevel = 64;
    resources.maxViewports = 16;
    resources.maxVertexAtomicCounters = 0;
    resources.maxTessControlAtomicCounters = 0;
    resources.maxTessEvaluationAtomicCounters = 0;
    resources.maxGeometryAtomicCounters = 0;
    resources.maxFragmentAtomicCounters = 8;
    resources.maxCombinedAtomicCounters = 8;
    resources.maxAtomicCounterBindings = 1;
    resources.maxVertexAtomicCounterBuffers = 0;
    resources.maxTessControlAtomicCounterBuffers = 0;
    resources.maxTessEvaluationAtomicCounterBuffers = 0;
    resources.maxGeometryAtomicCounterBuffers = 0;
    resources.maxFragmentAtomicCounterBuffers = 1;
    resources.maxCombinedAtomicCounterBuffers = 1;
    resources.maxAtomicCounterBufferSize = 16384;
    resources.maxTransformFeedbackBuffers = 4;
    resources.maxTransformFeedbackInterleavedComponents = 64;
    resources.maxCullDistances = 8;
    resources.maxCombinedClipAndCullDistances = 8;
    resources.maxSamples = 4;
    resources.limits.nonInductiveForLoops = 1;
    resources.limits.whileLoops = 1;
    resources.limits.doWhileLoops = 1;
    resources.limits.generalUniformIndexing = 1;
    resources.limits.generalAttributeMatrixVectorIndexing = 1;
    resources.limits.generalVaryingIndexing = 1;
    resources.limits.generalSamplerIndexing = 1;
    resources.limits.generalVariableIndexing = 1;
    resources.limits.generalConstantMatrixVectorIndexing = 1;
    return resources;
}