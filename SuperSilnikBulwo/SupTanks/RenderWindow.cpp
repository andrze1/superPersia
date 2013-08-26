#include "RenderWindow.h"

//-----------------------------------------------------------
//---------------------- constructor: -----------------------
//-----------------------------------------------------------
RenderWindow::RenderWindow() {
	if (debugInfoShowMode != DebugInfoShowMode_No) {
		debugInfo = new DebugInfo();
	}
	controller = new Controller();
	shaderManager = new ShaderManager(debugInfo);
	textureManager = new TextureManager();
	geometryRenderer = new GeometryRenderer(shaderManager, textureManager);
	
	// FPS timing initializations
	frameCount = 0;
	timeBase = 0;
	
	if (debugSceneMoving) {
		debugScreenMovingXPosition = 0.0f;
		debugScreenMovingYPosition = 0.0f;
		debugScreenMovingZPosition = 0.0f; 
		debugSceneXRotate = 0.0f;
		debugSceneYRotate = 0.0f;
	}
}
//-----------------------------------------------------------
//------------------------ methods: -------------------------
//-----------------------------------------------------------
void RenderWindow::initialize(int argc, char* argv[]) {
	try {
		// GLUT library initialization
		glutInit(&argc, argv);
   
		// frame buffer initialization
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   
		// set position of OpenGL window upper-left corner
		glutInitWindowPosition(startingWindowPositionX, startingWindowPositionY); 

		// initialize main application window dimensions
		glutInitWindowSize(startingScreenResolutionWidth, startingScreenResolutionHeight);
   
		// create application main window
		glutCreateWindow(mainWindowCaptionText);
	
		initializeGlutCallbackFunctions();
		openGlExtensionFunctionsSetup();

		// set clearing window screen color
		glClearColor(backgroundScreenColor[0], backgroundScreenColor[1], backgroundScreenColor[2], backgroundScreenColor[3]); 

		geometryRenderer->prepareGeometryRendering(); 
		textureManager->loadTextures();
		shaderManager->initializeVertexAndFragmentShaders();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	} catch (ResourceLoadException resourceLoadException) {
		processResourceLoadException(resourceLoadException);
	}
}

void RenderWindow::processResourceLoadException(ResourceLoadException& resourceLoadException) {
	debugInfo->printDebugInfo("Resource: \"" + resourceLoadException.resourceName + "\" not loaded!");
	int in;
	std::cin >> in;
	exit;
}

void RenderWindow::initializeGlutCallbackFunctions() {
	global_CurrentInstance = this;
	glutDisplayFunc(renderFrameCallback);
	glutReshapeFunc(resizeCallback);  
	glutSpecialFunc(specialKeyInputCallback);
}

// check and prepare selected OpenGL extensions handling. Loading all OpenGL extension functions used in application, not only used in RenderWindow class.
void RenderWindow::openGlExtensionFunctionsSetup() {
    // check OpenGL version
    const char* openGLversion = (char*)glGetString(GL_VERSION);
    std::string debugText = std::string("OpenGL version: ");
	debugText.append(openGLversion);
	debugText.append("\n\n");
	debugInfo->printDebugInfo(debugText);
	if ((openGLversion[0] < '1') || ((openGLversion[0] == '1') && (openGLversion[2] < '5')) || (openGLversion[1] != '.'))  {
		debugInfo->printDebugInfo(std::string("Error: bad OpenGL version format.\n"));
		exit(0);
    } else {
		// functions used in ShaderManager:
 		shaderManager->glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram"); 
		shaderManager->glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
		shaderManager->glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
		shaderManager->glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
		shaderManager->glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
		shaderManager->glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
		shaderManager->glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
		shaderManager->glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)wglGetProcAddress("glPatchParameteri");
		shaderManager->glGetObjectParameterivARB=(PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
		shaderManager->glGetInfoLogARB=(PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
		shaderManager->glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		shaderManager->glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f"); 
		shaderManager->glUniform4f = (PFNGLUNIFORM4FPROC) wglGetProcAddress("glUniform4f");
		shaderManager->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
		shaderManager->glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) wglGetProcAddress("glUniformMatrix3fv");
		shaderManager->glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
		// functions used in RenderWindow:
		// functions used by VBO:
		geometryRenderer->glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
		geometryRenderer->glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
		geometryRenderer->glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
		geometryRenderer->glMapBuffer = (PFNGLMAPBUFFERPROC) wglGetProcAddress("glMapBuffer");
		geometryRenderer->glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) wglGetProcAddress("glUnmapBuffer");
   }
}

void RenderWindow::applicationMainLoop() {
	glutMainLoop();
}

void RenderWindow::drawFPScounter() {
	frameCount++;
	timeV = glutGet(GLUT_ELAPSED_TIME);
	if (timeV - timeBase > 1000) {
		int fps = frameCount*1000.0/(timeV-timeBase);
	 	timeBase = timeV;
		frameCount = 0;

		char numstr[21];
		std::string debugText = std::string("FPS: ") + itoa(fps, numstr, 10) + std::string("\n");
		debugInfo->printDebugInfo(debugText);
	}
}

void RenderWindow::renderFrame() {
	renderFrameInitialSteps();
	frameMainRenderingAndLogic();
	renderFrameFinalSteps();
}

void RenderWindow::renderFrameInitialSteps() {
	// Clear screen to background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// use modelview matrix as the current matrix
    glMatrixMode(GL_MODELVIEW);
   
    // load identity matrix to the current (modelview) matrix
    glLoadIdentity();

	// debug screen moving (on keyboard arrows)
	if (debugSceneMoving) {
		// move screen horizontally
		glTranslatef(-debugScreenMovingSpeed*debugScreenMovingXPosition, 0.0, 0.0);
		// move screen vertically
		glTranslatef(0.0, -debugScreenMovingSpeed*debugScreenMovingYPosition, 0.0);
		// rotate screen by x axi
		glRotatef(debugSceneXRotate, 1.0, 0.0, 0.0);
		// rotate screen by y axi
		glRotatef(debugSceneYRotate, 0.0, 1.0, 0.0);
		// move scene farther or closer
		glTranslatef(1.5, 0.0, debugScreenMovingZPosition); 
	}
	
	#if 0 // for using fragment shader variables - not used
		ShaderManager::timeVar = 2;
		ShaderManager::glUniform1f(ShaderManager::uTime, ShaderManager::timeVar);
	#endif
	
	glEnable(GL_TEXTURE_2D); 
	
	if (geometryRenderMode == GeometryRenderMode_VBO) {
		// bind first buffer object
		geometryRenderer->glBindBuffer(GL_ARRAY_BUFFER, geometryRenderer->getVboIDs()[0]);
       
		// define vertex data table
		glVertexPointer(3, GL_FLOAT, 0, 0);
	}
}

void RenderWindow::renderFrameFinalSteps() {
	if (geometryRenderMode == GeometryRenderMode_VBO) {
		// disable buffer object binding
		geometryRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (debugInfoShowMode != DebugInfoShowMode_No) {
		drawFPScounter();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

void RenderWindow::frameMainRenderingAndLogic() {
	// FOR TESTING:
		// draw textured rectangles:
		geometryRenderer->drawTexturedRectangle(0, 0, textureManager->getTankRedTopTexture());
		geometryRenderer->drawTexturedRectangle(100, 20);
		geometryRenderer->drawTexturedRectangle(200, 50, textureManager->getGrassTexture());
		geometryRenderer->drawTexturedRectangle(0, 400, textureManager->getSkyTexture());
		
		// draw colored rectangles:
		static int frameCount = 0;
		if (frameCount < 255) {
			frameCount += 4;
		} else {
			frameCount = 0;
		}
		GLbyte color[3] = {frameCount, 255-frameCount, 0};
		geometryRenderer->drawColoredRectangle(10, 0, color[0], color[1], color[2], 250, 350);
		
		static int position = 0;
		if (position < 600) {
			position +=4;
		} else {
			position = 0;
		}
		geometryRenderer->drawColoredRectangle(780, position, 255, 0, 0, 10, 50);

		geometryRenderer->drawColoredRectangle(1024-24, 600-24, 0, 0, 255, 24, 24);
	// END OF TESTING
}

// OpenGL window reshape routine.
void RenderWindow::resize(int width, int height) {
	// Set viewport size to be entire OpenGL window.
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  
	// Set matrix mode to projection.
	glMatrixMode(GL_PROJECTION);

	// Clear current projection matrix to identity.
	glLoadIdentity();

	// Specify the orthographic (or perpendicular) projection, 
	// i.e., define the viewing box.
	if (viewMode == ViewMode_Orthographic) {
		glOrtho(0, width, height, 0, -1, 1);
	} else if (viewMode == ViewMode_Frustum) {
		glFrustum(0, width/10, height/10, 0, -10, 1.0);
	}
	
	// Set matrix mode to modelview.
	glMatrixMode(GL_MODELVIEW);
   
	// Clear current modelview matrix to identity.
	glLoadIdentity();
}

void RenderWindow::specialKeyInput(int keyId, int x, int y) {
	if (debugSceneMoving) {
		handleDebugScreenMovingKeyInput(keyId);
	}
   glutPostRedisplay();
}

void RenderWindow::handleDebugScreenMovingKeyInput(int keyId) {
	if (keyId == GLUT_KEY_UP) {
		debugScreenMovingYPosition += 1;
	}
	if (keyId == GLUT_KEY_DOWN) {
		debugScreenMovingYPosition -= 1;
	}
	if (keyId == GLUT_KEY_LEFT) {
		debugScreenMovingXPosition += 1;
	}
	if (keyId == GLUT_KEY_RIGHT) {
		debugScreenMovingXPosition -= 1;
	}
	if (keyId == GLUT_KEY_PAGE_UP) {
		debugScreenMovingZPosition -= 0.1f;
	}
	if (keyId == GLUT_KEY_PAGE_DOWN) {
		if (debugScreenMovingZPosition < 3.0) {
			debugScreenMovingZPosition += 0.1f;
		}
	}
	if (keyId == GLUT_KEY_HOME) {
		debugSceneYRotate -= 5.0f;
	}
	if (keyId == GLUT_KEY_END) {
		debugSceneYRotate += 5.0f;
	}
	if (debugSceneXRotate > 356.0f) {
		debugSceneXRotate = 0.0f;
	}
	if (debugSceneXRotate < -1.0f) {
		debugSceneXRotate = 355.0f;
	}
	if (debugSceneYRotate > 356.0f) {
		debugSceneYRotate = 0.0f;
	}
	if (debugSceneYRotate < -1.0f) {
		debugSceneYRotate = 355.0f;
	}
}

//-----------------------------------------------------------
//-------------------- global functions: --------------------
//-----------------------------------------------------------
extern "C" void renderFrameCallback() {
	global_CurrentInstance->renderFrame();
}
extern "C" void resizeCallback(int width, int height) {
	global_CurrentInstance->resize(width, height);
}
extern "C" void specialKeyInputCallback(int key, int x, int y) {
	global_CurrentInstance->specialKeyInput(key, x, y);
}
