#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/glut.h>
#include "GL/glext.h"
#include <GL/GLU.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

#include "Texture.h"
#include "ResourceLoadException.h"

// bitmap file struct
struct BitMapFile {
   int width;
   int height;
   unsigned char *pixelData; // r1, g1, b1, r2, g2, b2...
};

class TextureManager {
	private:
		static const int textureCount = 3;

		BitMapFile *getBMPData(std::string filename);
		unsigned int textureIDs[textureCount];
		unsigned int textureIDsIndex;
		
		std::string grassTextureFileRelativePath;
		std::string skyTextureFileRelativePath;
		std::string tankRedTopFileRelativePath;

		Texture grassTexture;
		Texture skyTexture;
		Texture tankRedTopTexture;

		// methods:
		void loadTexture(Texture& texture, std::string textureFileName);
	
	public:
		// accessors:
		Texture getGrassTexture();
		Texture getSkyTexture();
		Texture getTankRedTopTexture();
		
		// methods:
		void loadTextures();

		// constructor:
		TextureManager();
};

#endif
