#include "pch.h"
#include "GLSLCompiler.h"
//#include <glslang/SPIRV/GlslangToSpv.h>
//#include <glslang/Include/glslang_c_interface.h>
GLSLCompiler::GLSLCompiler()
{
	//int v = glslang::GetSpirvGeneratorVersion();
//	glslang_initialize_process();
	//log.Debug(std::to_string());
	//glslang::InitializeProcess();
	
}

std::vector<unsigned> GLSLCompiler::CompileGLSL(const std::string& Source)
{
	std::vector<unsigned> SPV;
	//glslang::TIntermediate intermi;
	//glslang::GlslangToSpv(vk::ShaderStageFlagBits::eVertex,)
	return SPV;
}
