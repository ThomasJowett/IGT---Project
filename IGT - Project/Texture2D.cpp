#include "Texture2D.h"
#include <iostream>
#include <fstream>

extern std::string gApplicationLocation;

static std::map<std::string, GLuint> mTextures;

GLuint Texture2D::LoadTexture2D(const char* path)
{	
	GLuint ID;
	//load the image onto a surface
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		std::cerr << "Unable to create Image. Error: " << SDL_GetError() << std::endl;
		surface = IMG_Load((gApplicationLocation + "/Images/NULL.png").c_str()); // make sure to have the nullptr texture in the project directory
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenTextures(1, &ID); //Get the next texture ID
	glBindTexture(GL_TEXTURE_2D, ID); //Bind the texture to the ID.
	
	//Set some parameters so it renders correctly
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//create the texture from the surface pixels
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	//clear the surface we don't need it anymore
	SDL_FreeSurface(surface);

	glBindTexture(GL_TEXTURE_2D, 0);

	mTextures.insert(std::pair<std::string, GLuint>{path, ID});

	return ID;
}

GLuint Texture2D::LoadTexture2DRaw(const char * path, int width, int height)
{
	GLuint ID;
	char* tempTextureData;
	int fileSize;
	std::ifstream inFile;
	inFile.open(path, std::ios::binary);

	if (!inFile.good())
	{
		std::cerr << "Can't open texture file " << path << std::endl;
		return LoadTexture2D((gApplicationLocation + "/Images/NULL.png").c_str()); // make sure to have the nullptr texture in the project directory
	}

	inFile.seekg(0, std::ios::end); // Seek to end of file
	fileSize = (int)inFile.tellg(); //	Get current position in file - The End, this givesus the total file size
	tempTextureData = new char[fileSize]; // Create a new array to store the data (of the correct size)
	inFile.seekg(0, std::ios::beg); // Seek back to beginning of the file.
	inFile.read(tempTextureData, fileSize); // Read in all the data.
	inFile.close(); //Close the file.

	glGenTextures(1, &ID); //Get the next texture ID
	glBindTexture(GL_TEXTURE_2D, ID); //Bind the texture to the ID.

	//Set some parameters so it renders correctly
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);
	delete[] tempTextureData; // Clear up the data - we don't need it anymore
	glBindTexture(GL_TEXTURE_2D, 0);

	mTextures.insert(std::pair<std::string, GLuint>{path, ID});

	return ID;
}

void Texture2D::DeleteTexture(const char* path)
{
	glDeleteTextures(1, &mTextures.at(path));
	mTextures.erase(path);
}

void Texture2D::DeleteAllTextures()
{
	std::map<std::string, GLuint>::iterator it = mTextures.begin();
	while (it != mTextures.end())
	{
		glDeleteTextures(1, &it->second);
		it++;
	}

	mTextures.clear();
}

GLuint Texture2D::GetTexture2D(const char * path)
{
	std::string fullPath = gApplicationLocation + '/' + path;
	if (mTextures.find(fullPath) == mTextures.end())
	{
		return LoadTexture2D(fullPath.c_str());
	}
	else
	{
		return mTextures.at(fullPath);
	}
}
