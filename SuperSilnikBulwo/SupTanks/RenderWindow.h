#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <GL/glut.h>
#include "GL/glext.h"
#include <GL/GLU.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

#include "Controller.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GeometryRenderer.h"
#include "DebugInfo.h"
#include "Enums.h"
#include "Constants.h"

//-----------------------------------------------------------
//------------------- class RenderWindow --------------------
//-----------------------------------------------------------
class RenderWindow {
	private:
		ShaderManager* shaderManager;
		GeometryRenderer* geometryRenderer;
		TextureManager* textureManager;
		DebugInfo* debugInfo;
		Controller* controller;

		// debug variables: used if debugSceneMoving is true
			float debugScreenMovingXPosition;
			float debugScreenMovingYPosition;
			float debugScreenMovingZPosition; 
			float debugSceneXRotate;
			float debugSceneYRotate;
		// end of debug variables

		// for FPS timing:
		int frameCount;
		int timeV;
		int timeBase;

		void initializeGlutCallbackFunctions();
		void openGlExtensionFunctionsSetup();
		void handleDebugScreenMovingKeyInput(int keyId);
		void drawFPScounter();
		void renderFrameInitialSteps();
		void frameMainRenderingAndLogic();
		void renderFrameFinalSteps();
		void processResourceLoadException(ResourceLoadException& resourceLoadException);

	public:
		// methods
		void initialize(int argc, char* argv[]);
		void applicationMainLoop();
		
		// methods used as glut callback functions
		void renderFrame();
		void resize(int width, int height);
		void specialKeyInput(int key, int x, int y);
		
		RenderWindow();
};

// used for glut callback functions. Because OpenGL and GLUT are pure C api, so we have to "convert" class methods to C functions.
// according to: http://stackoverflow.com/questions/3589422/using-opengl-glutdisplayfunc-within-class
static RenderWindow* global_CurrentInstance;
extern "C" void renderFrameCallback();
extern "C" void resizeCallback(int width, int height);
extern "C" void specialKeyInputCallback(int key, int x, int y);

#endif // RENDERWINDOW_H
