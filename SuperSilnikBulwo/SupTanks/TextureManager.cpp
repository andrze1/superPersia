#include "TextureManager.h"

//-----------------------------------------------------------
//---------------------- constructor: -----------------------
//-----------------------------------------------------------
TextureManager::TextureManager() {
	textureIDsIndex = 0;
	std::string texturesFolderRelativePath = "textures/";
	grassTextureFileRelativePath = texturesFolderRelativePath + "grass.bmp";
	skyTextureFileRelativePath = texturesFolderRelativePath + "sky.bmp";
	tankRedTopFileRelativePath = texturesFolderRelativePath + "tank-red-top.bmp";
}

//-----------------------------------------------------------
//----------------------- accessors: ------------------------
//-----------------------------------------------------------
Texture TextureManager::getGrassTexture() {
	return grassTexture;
}
Texture TextureManager::getSkyTexture() {
	return skyTexture;
}
Texture TextureManager::getTankRedTopTexture() {
	return tankRedTopTexture;
}

//-----------------------------------------------------------
//------------------------ methods: -------------------------
//-----------------------------------------------------------
// works only for uncompressed bmp files of 24-bit color
BitMapFile* TextureManager::getBMPData(std::string fileName) {
	BitMapFile *bmp = new BitMapFile;
    unsigned int size, offset, headerSize;
  
    // read input file name.
    std::ifstream infile(fileName.c_str(), std::ios::binary);
 
    // if the file doesn't exist
	if (!infile.is_open()) {
	    throw ResourceLoadException(fileName);
    }

    // get the starting point of the image data
    infile.seekg(10);
    infile.read((char*)&offset, 4); 
    
    // get the header size of the bitmap
    infile.read((char*)&headerSize,4);
 
    // get width and height values in the bitmap header
    infile.seekg(18);
    infile.read((char*)&bmp->width, 4);
    infile.read((char*)&bmp->height, 4);
 
    // allocate buffer for the image
    size = bmp->width * bmp->height * 24;
    bmp->pixelData = new unsigned char[size];
 
    // read bitmap data
    infile.seekg(offset);
    infile.read((char*)bmp->pixelData, size);
    
    // reverse color from bgr to rgb
    int temp;
    for (unsigned int i = 0; i < size; i += 3) { 
		temp = bmp->pixelData[i]; 
 	    bmp->pixelData[i] = bmp->pixelData[i+2];
	    bmp->pixelData[i+2] = temp;
	}
    return bmp;
}

void TextureManager::loadTextures() {	
	// preparing identifiers of texture objects
    glGenTextures(textureCount, textureIDs);
	
	loadTexture(grassTexture, grassTextureFileRelativePath);
	loadTexture(skyTexture, skyTextureFileRelativePath);
	loadTexture(tankRedTopTexture, tankRedTopFileRelativePath);
}

void TextureManager::loadTexture(Texture& texture, std::string textureFileName) {
	texture.id = textureIDs[textureIDsIndex];
	textureIDsIndex++;
	BitMapFile* image = getBMPData(textureFileName);
	texture.width = image->width;
	texture.height = image->height;
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixelData);
}
