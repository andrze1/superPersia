#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <GL/glut.h>
#include "GL/glext.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>

#include "Constants.h"
#include "DebugInfo.h"
#include "ResourceLoadException.h"

class ShaderManager {
	private:
		DebugInfo* debugInfo;
		std::string vertexShaderFileRelativePath;
		std::string fragmentShaderForTexturesFileRelativePath;
		std::string fragmentShaderForColoredRectanglesFileRelativePath;

		// identifiers of shader programs
		GLint shaderSuperiorForTexturesHandle; // shaders superior identifier, when textures drawn
		GLint shaderSuperiorForColoredRectanglesHandle; // shaders superior identifier, when colored rectangles drawn
		GLint vertexShaderHandle;
		GLint fragmentShaderForTexturesHandle;
		GLint fragmentShaderForColoredRectanglesHandle;

		// handles to vertex shaders variables
		GLint xPositionShaderForTexturesVariableHandle;
		GLint yPositionShaderForTexturesVariableHandle;
		GLint xPositionShaderForColoredRectanglesVariableHandle;
		GLint yPositionShaderForColoredRectanglesVariableHandle;
		// handles to fragment shaders variables
		GLint fragColorVariableHandle;
		
		// methods:
		char* readShaderTextFromFile(std::string shaderFileName);
		void checkShaderCompilingAndLinkingErrors(GLint shaderHandle, GLint& errorStatus);
	
	public:
		// pointers for OpenGL extensions functions used by using shaders
		PFNGLCREATEPROGRAMPROC glCreateProgram;
		PFNGLCREATESHADERPROC glCreateShader;
		PFNGLSHADERSOURCEPROC glShaderSource;
		PFNGLATTACHSHADERPROC glAttachShader;
		PFNGLLINKPROGRAMPROC glLinkProgram;
		PFNGLUSEPROGRAMPROC	glUseProgram;
		PFNGLCOMPILESHADERPROC	glCompileShader;
		PFNGLPATCHPARAMETERIPROC	glPatchParameteri;
		PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
		PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
		PFNGLGETUNIFORMLOCATIONPROC  glGetUniformLocation;
		PFNGLUNIFORM1FPROC glUniform1f;
		PFNGLUNIFORM4FPROC glUniform4f;
		PFNGLUNIFORM1IPROC glUniform1i; 
		PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
		PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
		
		// accessors:
		GLint getShaderSuperiorForTexturesHandle();
		GLint getShaderSuperiorForColoredRectanglesHandle();
		GLint getFragmentShaderForTexturesHandle();
		GLint getFragmentShaderForColoredRectanglesHandle();
		GLint getXPositionShaderForTexturesVariableHandle();
		GLint getYPositionShaderForTexturesVariableHandle();
		GLint getXPositionShaderForColoredRectanglesVariableHandle();
		GLint getYPositionShaderForColoredRectanglesVariableHandle();
		GLint getFragColorVariableHandle();

		// methods:
		void initializeVertexAndFragmentShaders();
		
		// constructor:
		ShaderManager(DebugInfo* debugInfo);
};

#endif
