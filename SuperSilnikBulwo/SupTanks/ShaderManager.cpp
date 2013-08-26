#include "ShaderManager.h"

//-----------------------------------------------------------
//---------------------- constructor: -----------------------
//-----------------------------------------------------------
ShaderManager::ShaderManager(DebugInfo* debugInfo) {
	ShaderManager::debugInfo = debugInfo;

	shaderSuperiorForTexturesHandle = 0;
	shaderSuperiorForColoredRectanglesHandle = 0;
	vertexShaderHandle = 0;
	fragmentShaderForTexturesHandle = 0;
	fragmentShaderForColoredRectanglesHandle = 0;

	glCreateProgram = NULL;
	glCreateShader = NULL;
	glShaderSource = NULL;
	glAttachShader = NULL;
	glLinkProgram = NULL;
	glUseProgram = NULL;
	glCompileShader = NULL;
	glPatchParameteri = NULL;
	glGetObjectParameterivARB = NULL;
	glGetInfoLogARB = NULL;
	glGetUniformLocation = NULL;
	glUniform1f = NULL; 
	glUniform4f = NULL;
	glUniform1i = NULL; 
	glUniformMatrix4fv = NULL;
	glUniformMatrix3fv = NULL;

	std::string texturesFolderRelativePath = "shaders/";
	vertexShaderFileRelativePath = texturesFolderRelativePath + "VertexShader.vs";
	fragmentShaderForTexturesFileRelativePath = texturesFolderRelativePath + "FragmentShader_Textures.fs";	
	fragmentShaderForColoredRectanglesFileRelativePath = texturesFolderRelativePath + "FragmentShader_ColoredRectangles.fs";	
}

//-----------------------------------------------------------
//----------------------- accessors: ------------------------
//-----------------------------------------------------------
GLint ShaderManager::getShaderSuperiorForTexturesHandle() {
	return shaderSuperiorForTexturesHandle;
}
GLint ShaderManager::getShaderSuperiorForColoredRectanglesHandle() {
	return shaderSuperiorForColoredRectanglesHandle;
}
GLint ShaderManager::getFragmentShaderForTexturesHandle() {
	return fragmentShaderForTexturesHandle;
}
GLint ShaderManager::getFragmentShaderForColoredRectanglesHandle() {
	return fragmentShaderForColoredRectanglesHandle;
}
GLint ShaderManager::getXPositionShaderForTexturesVariableHandle() {
	return xPositionShaderForTexturesVariableHandle;
}
GLint ShaderManager::getYPositionShaderForTexturesVariableHandle() {
	return yPositionShaderForTexturesVariableHandle;
}
GLint ShaderManager::getXPositionShaderForColoredRectanglesVariableHandle() {
	return xPositionShaderForColoredRectanglesVariableHandle;
}
GLint ShaderManager::getYPositionShaderForColoredRectanglesVariableHandle() {
	return yPositionShaderForColoredRectanglesVariableHandle;
}
GLint ShaderManager::getFragColorVariableHandle() {
	return fragColorVariableHandle;
}

//-----------------------------------------------------------
//------------------------ methods: -------------------------
//-----------------------------------------------------------
void ShaderManager::initializeVertexAndFragmentShaders() {
    char* vertexShaderText = readShaderTextFromFile(vertexShaderFileRelativePath);
    char* fragmentShaderForTexturesText = readShaderTextFromFile(fragmentShaderForTexturesFileRelativePath);
	char* fragmentShaderForColoredRectanglesText = readShaderTextFromFile(fragmentShaderForColoredRectanglesFileRelativePath);

    shaderSuperiorForTexturesHandle = glCreateProgram(); // creating shader superior handle
	shaderSuperiorForColoredRectanglesHandle = glCreateProgram(); // creating shader superior handle
    vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER); // creating fragment shader handle
    fragmentShaderForTexturesHandle = glCreateShader(GL_FRAGMENT_SHADER); // creating fragment shader handle
	fragmentShaderForColoredRectanglesHandle = glCreateShader(GL_FRAGMENT_SHADER); // creating fragment shader handle

    glShaderSource(vertexShaderHandle, 1, (const char**) &vertexShaderText, NULL);
    glShaderSource(fragmentShaderForTexturesHandle, 1, (const char**) &fragmentShaderForTexturesText, NULL);
	glShaderSource(fragmentShaderForColoredRectanglesHandle, 1, (const char**) &fragmentShaderForColoredRectanglesText, NULL);

    glCompileShader(vertexShaderHandle); 
    glCompileShader(fragmentShaderForTexturesHandle); 
	glCompileShader(fragmentShaderForColoredRectanglesHandle); 
 
    // error check
	GLint errorStatus = 0;
	checkShaderCompilingAndLinkingErrors(vertexShaderHandle, errorStatus);
	checkShaderCompilingAndLinkingErrors(fragmentShaderForTexturesHandle, errorStatus);
	checkShaderCompilingAndLinkingErrors(fragmentShaderForColoredRectanglesHandle, errorStatus);
		
    // attach shaders to shader superior handle
    glAttachShader(shaderSuperiorForTexturesHandle, vertexShaderHandle); 
	glAttachShader(shaderSuperiorForTexturesHandle, fragmentShaderForTexturesHandle); 
	glAttachShader(shaderSuperiorForColoredRectanglesHandle, vertexShaderHandle); 
	glAttachShader(shaderSuperiorForColoredRectanglesHandle, fragmentShaderForColoredRectanglesHandle); 
    
    glLinkProgram(shaderSuperiorForTexturesHandle); 
	glLinkProgram(shaderSuperiorForColoredRectanglesHandle); 
	
	checkShaderCompilingAndLinkingErrors(shaderSuperiorForTexturesHandle, errorStatus);
	checkShaderCompilingAndLinkingErrors(shaderSuperiorForColoredRectanglesHandle, errorStatus);

	// get handles to vertex shaders uniform variables
    xPositionShaderForTexturesVariableHandle = glGetUniformLocation(shaderSuperiorForTexturesHandle, "xPosition");
    yPositionShaderForTexturesVariableHandle = glGetUniformLocation(shaderSuperiorForTexturesHandle, "yPosition");
	xPositionShaderForColoredRectanglesVariableHandle = glGetUniformLocation(shaderSuperiorForColoredRectanglesHandle, "xPosition");
    yPositionShaderForColoredRectanglesVariableHandle = glGetUniformLocation(shaderSuperiorForColoredRectanglesHandle, "yPosition");
	// get handles to fragment shaders uniform variables
	fragColorVariableHandle = glGetUniformLocation(shaderSuperiorForColoredRectanglesHandle, "fragColor");
}

char* ShaderManager::readShaderTextFromFile(std::string shaderFileName) {
   FILE* filePointer = fopen(shaderFileName.c_str(), "rb");	
   if (filePointer == NULL) {
	   throw ResourceLoadException(shaderFileName);
   }
   
   char* shaderText = NULL;
   long shaderTextLenght = 0;

   fseek(filePointer, 0L, SEEK_END);
   shaderTextLenght = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   shaderText = (char*) malloc((shaderTextLenght+1) * sizeof(char)); 
   fread(shaderText, 1, shaderTextLenght, filePointer);
   shaderText[shaderTextLenght] = '\0';
   fclose(filePointer);
   return shaderText;
}

void ShaderManager::checkShaderCompilingAndLinkingErrors(GLint shaderHandle, GLint& errorStatus) {
	glGetObjectParameterivARB(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &errorStatus);
	if (!errorStatus) {
		if (debugInfoShowMode != DebugInfoShowMode_No) {
			const int maxInfoLogSize=2048;
			GLchar infoLog[maxInfoLogSize];
			glGetInfoLogARB(shaderHandle, maxInfoLogSize, NULL, infoLog);
			debugInfo->printDebugInfo(infoLog);
		}
	}
}
