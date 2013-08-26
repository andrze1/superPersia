#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <GL/glut.h>
#include "GL/glext.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Enums.h"
#include "Constants.h"

//-----------------------------------------------------------
//----------------- class GeometryRenderer ------------------
//-----------------------------------------------------------
class GeometryRenderer {

	private:
		unsigned int latestShownTextureId;
		int latestRectangleXdimensionWhenColoredRectangleDrawn;
		int latestRectangleYdimensionWhenColoredRectangleDrawn;

		LatelyRenderedTexturedOrColoredRectangle latelyRenderedTexturedOrColoredRectangle;

		ShaderManager* shaderManager;
		TextureManager* textureManager;

		// vertex array for drawing every textured rectangles
		GLfloat rectangleVertexArray[12];
		
		// texture UVs array
		GLfloat rectangleTexUVarray[8];

		// array for storing IDs of VBO objects
		GLuint vboIDs[2];

		// methods:
		void initializeTextureGeometryToDraw(unsigned int width, unsigned int height);
		void initializeRectangleVertexArray(unsigned int width, unsigned int height);
		inline void drawTexturedRectangleStep2(int x, int y);
		inline void drawColoredRectangleStep2(int x, int y, GLubyte r, GLubyte g, GLubyte b);
	public:
		// pointers for OpenGL extensions functions used in geometry renderer
		PFNGLGENBUFFERSPROC glGenBuffers;
		PFNGLBINDBUFFERPROC glBindBuffer;
		PFNGLBUFFERDATAPROC glBufferData;
		PFNGLMAPBUFFERPROC glMapBuffer;
		PFNGLUNMAPBUFFERPROC glUnmapBuffer;

		// accessors:
		GLuint* getVboIDs();

		// methods:
		void prepareGeometryRendering();
		void drawGameBoard(float& xPos, float& yPos);
		void drawTexturedRectangle(int x, int y, Texture& texture);
		void drawTexturedRectangle(int x, int y);
		void drawColoredRectangle(int x, int y, GLubyte r, GLubyte g, GLubyte b, unsigned int width, unsigned int height);
		void drawColoredRectangle(int x, int y, GLubyte r, GLubyte g, GLubyte b);
		
		// constructor:
		GeometryRenderer(ShaderManager* shaderManager, TextureManager* textureManager);
};

#endif
