#pragma once
#include "IInternalModule.h"
#include "Debug/Debug.h"

	
	class GLSLCompiler : Landmark::Systems::IInternalModule <GLSLCompiler>
	{
		
	private:
	Log log = Log("GLSL Compiler");
		

	public:
		GLSLCompiler();
		std::vector<unsigned int> CompileGLSL(const std::string& Source);
	};

	

