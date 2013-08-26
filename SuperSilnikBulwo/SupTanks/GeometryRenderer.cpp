#include "GeometryRenderer.h"

//-----------------------------------------------------------
//---------------------- constructor: -----------------------
//-----------------------------------------------------------
GeometryRenderer::GeometryRenderer(ShaderManager* shaderManager, TextureManager* textureManager) {
	this->shaderManager = shaderManager;
	this->textureManager = textureManager;

	glGenBuffers = NULL;
	glBindBuffer = NULL;
	glBufferData = NULL;
	glMapBuffer = NULL;
	glUnmapBuffer = NULL;

	float initialRectangleXDimension = 128;
	float initialRectangleYDimension = 64;
		
	// bottom left corner
	rectangleVertexArray[0] = 0; // x (lef-right)
	rectangleVertexArray[1] = 0; // y (top-bottom)
	rectangleVertexArray[2] = 0; // z (far-near)
	// bottom right corner
	rectangleVertexArray[3] = 0;
	rectangleVertexArray[4] = initialRectangleYDimension;
	rectangleVertexArray[5] = 0; 
	// top rigth corner
	rectangleVertexArray[6] = initialRectangleXDimension;
	rectangleVertexArray[7] = initialRectangleYDimension;
	rectangleVertexArray[8] = 0;
	// top left corner
	rectangleVertexArray[9] = initialRectangleXDimension;
	rectangleVertexArray[10] = 0;
	rectangleVertexArray[11] = 0;
		
	// that numbers order, in order to proper texture displaying
	rectangleTexUVarray[0] = 0.0;
	rectangleTexUVarray[1] = 1.0;
	rectangleTexUVarray[2] = 0.0;
	rectangleTexUVarray[3] = 0.0;

	rectangleTexUVarray[4] = 1.0;
	rectangleTexUVarray[5] = 0.0;
	rectangleTexUVarray[6] = 1.0;
	rectangleTexUVarray[7] = 1.0;	

	latestShownTextureId = -1;
	latestRectangleXdimensionWhenColoredRectangleDrawn = 0;
	latestRectangleYdimensionWhenColoredRectangleDrawn = 0;

	latelyRenderedTexturedOrColoredRectangle = LatelyRenderedTexturedOrColoredRectangle_NotRenderedYet;
}

//-----------------------------------------------------------
//----------------------- accessors: ------------------------
//-----------------------------------------------------------
GLuint* GeometryRenderer::getVboIDs() {
	return vboIDs;
}
//-----------------------------------------------------------
//------------------------ methods: -------------------------
//-----------------------------------------------------------
void GeometryRenderer::prepareGeometryRendering() {
	#if 0
	// wy³¹czenie Vertical Synchronization. Jeœli nie jest wy³¹czone, to jest ograniczenie fps'ów aplikacji do max. 60.
	// Aby dzia³a³o, potrzeba biblioteki: #include <GL/glfw.h> (nie próbowa³em).
	//glfwSwapInterval(0);
	#endif

	glEnable(GL_DEPTH_TEST); 
	    
	if (geometryRenderMode == GeometryRenderMode_Normal) {
		glEnableClientState(GL_VERTEX_ARRAY); // enable vertex arrays handling
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//glEnableClientState(GL_NORMAL_ARRAY); // in order to enable lighting
	} else if (geometryRenderMode == GeometryRenderMode_VBO) {
		// generate two identificators of buffer objects
		glGenBuffers(2, vboIDs);
			
		// bind first buffer object
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);

		// load data to first buffer object
		glBufferData(GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4 * 3, rectangleVertexArray, GL_STATIC_DRAW);

		// disable buffer objects binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
		
	// bind prepared vertex data to current buffer object.
	// -> GLint size = 3, because 3 numbers to describe each vertex (coordinates)
	// -> GLenum type = GL_FLOAT, because vertex coordinates are GLfloats
	// -> GLsizei stride = 0, because coordinates of each vertex are each directly after previous
	glVertexPointer(3, GL_FLOAT, 0, rectangleVertexArray);
		
		
	// in order to use textures on geometry to be drawn
	glTexCoordPointer(2, GL_FLOAT, 0, rectangleTexUVarray);

	// not used because of no lighting
	//glNormalPointer(GL_FLOAT, 0, cubeVertexArray);
}

// TODO: udoskonaliæ t¹ funkcjê, by wyœwietla³a tylko ten fragment mapy, który ma byæ wyœwietlony. Przy rysowaniu mapy najlepiej nie korzystaæ z funkcji rysuj¹cej osobny oteksturowany prostok¹t.
void GeometryRenderer::drawGameBoard(float& xPos, float& yPos) {
	for (int i = 0; i < 63; ++i) {
		xPos = 0;
		for (int j = 0; j < 63; ++j) {
			//if (geometryRenderMode == GeometryRenderMode_Normal) {
				//glDrawArrays(GL_QUADS, 0, 3*4);
			//}
			//if (geometryRenderMode == GeometryRenderMode_VBO) {
				glDrawArrays( GL_QUADS, 0, 4 );
			//}
			xPos += 0.03f;
			//shaderManager->glUniform1f(shaderManager->getXPositionShaderVariableHandle(), xPos);
		}
		yPos += 0.03f;
		//shaderManager->glUniform1f(shaderManager->getYPositionShaderVariableHandle(), yPos);
	}
}

void GeometryRenderer::drawTexturedRectangle(int x, int y, Texture& texture) {
	if (latelyRenderedTexturedOrColoredRectangle == LatelyRenderedTexturedOrColoredRectangle_Colored || latelyRenderedTexturedOrColoredRectangle == LatelyRenderedTexturedOrColoredRectangle_NotRenderedYet) {
		// switching current shader superior to shader superior for textures rendering
		shaderManager->glUseProgram(shaderManager->getShaderSuperiorForTexturesHandle()); 
		latelyRenderedTexturedOrColoredRectangle = LatelyRenderedTexturedOrColoredRectangle_Textured;
	}
	if (texture.id != latestShownTextureId) {
		initializeTextureGeometryToDraw(texture.width, texture.height);
		latestShownTextureId = texture.id;
		glBindTexture(GL_TEXTURE_2D, texture.id);
	}
	drawTexturedRectangleStep2(x, y);
}
void GeometryRenderer::drawTexturedRectangle(int x, int y) {
	drawTexturedRectangleStep2(x, y);
}
inline void GeometryRenderer::drawTexturedRectangleStep2(int x, int y) {
	shaderManager->glUniform1f(shaderManager->getXPositionShaderForTexturesVariableHandle(), (GLfloat)x);
	shaderManager->glUniform1f(shaderManager->getYPositionShaderForTexturesVariableHandle(), (GLfloat)y);
	glDrawArrays(GL_QUADS, 0, 4);
}

void GeometryRenderer::drawColoredRectangle(int x, int y, GLubyte r, GLubyte g, GLubyte b, unsigned int width, unsigned int height) {
	if (latelyRenderedTexturedOrColoredRectangle == LatelyRenderedTexturedOrColoredRectangle_Textured || latelyRenderedTexturedOrColoredRectangle == LatelyRenderedTexturedOrColoredRectangle_NotRenderedYet) {
		glBindTexture(GL_TEXTURE_2D, NULL); // disable texture displaying
		// switching current shader superior to shader superior for colored rectangles rendering
		shaderManager->glUseProgram(shaderManager->getShaderSuperiorForColoredRectanglesHandle()); 
		latelyRenderedTexturedOrColoredRectangle = LatelyRenderedTexturedOrColoredRectangle_Colored;
	}
	if (width != latestRectangleXdimensionWhenColoredRectangleDrawn || height != latestRectangleYdimensionWhenColoredRectangleDrawn) {
		initializeTextureGeometryToDraw(width, height);
	}
	latestRectangleXdimensionWhenColoredRectangleDrawn = width;
	latestRectangleYdimensionWhenColoredRectangleDrawn = height;

	drawColoredRectangleStep2(x, y, r, g, b);
}
void GeometryRenderer::drawColoredRectangle(int x, int y, GLubyte r, GLubyte g, GLubyte b) {
	drawColoredRectangleStep2(x, y, r, g, b);
}
inline void GeometryRenderer::drawColoredRectangleStep2(int x, int y, GLubyte r, GLubyte g, GLubyte b) {
	// initialize vertex shader uniform variables
	shaderManager->glUniform1f(shaderManager->getXPositionShaderForColoredRectanglesVariableHandle(), (GLfloat)x);
	shaderManager->glUniform1f(shaderManager->getYPositionShaderForColoredRectanglesVariableHandle(), (GLfloat)y);
	// initialize fragment shader uniform variable
	shaderManager->glUniform4f(shaderManager->getFragColorVariableHandle(), (GLfloat)r/256.f, (GLfloat)g/256.f, (GLfloat)b/256.f, 1.f);
	glDrawArrays(GL_QUADS, 0, 4);
}

void GeometryRenderer::initializeTextureGeometryToDraw(unsigned int width, unsigned int height) {
	initializeRectangleVertexArray(width, height);

	// bind first buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);

	// load data to first buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4 * 3, rectangleVertexArray, GL_STATIC_DRAW);

	// disable buffer objects binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	// bind prepared vertex data to current buffer object.
	// -> GLint size = 3, because 3 numbers to describe each vertex (coordinates)
	// -> GLenum type = GL_FLOAT, because vertex coordinates are GLfloats
	// -> GLsizei stride = 0, because coordinates of each vertex are each directly after previous
	glVertexPointer(3, GL_FLOAT, 0, rectangleVertexArray);
}

void GeometryRenderer::initializeRectangleVertexArray(unsigned int width, unsigned int height) {
	// similarly as in initializations in class constructor
	rectangleVertexArray[6] = rectangleVertexArray[9] = (GLfloat)width;
	rectangleVertexArray[4] = rectangleVertexArray[7] = (GLfloat)height;
}
