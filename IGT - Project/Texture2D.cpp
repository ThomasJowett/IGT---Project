#include "Texture2D.h"
#include <iostream>
#include <fstream>

using namespace::std;

static std::vector<GLuint> mTextureIDs;

GLuint Texture2D::LoadTexture2D(const char* path)
{	
	GLuint ID;
	//load the image onto a surface
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		std::cerr << "Unable to create Image. Error: " << SDL_GetError() << std::endl;
		surface = IMG_Load("Images/NULL.png"); // make sure to have the null texture in the project directory
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

	mTextureIDs.push_back(ID);

	return ID;
}

GLuint Texture2D::LoadTexture2DRaw(const char * path, int width, int height)
{
	GLuint ID;
	char* tempTextureData;
	int fileSize;
	ifstream inFile;
	inFile.open(path, ios::binary);

	if (!inFile.good())
	{
		cerr << "Can't open texture file " << path << endl;
		return LoadTexture2D("Images/NULL.png"); // make sure to have the null texture in the project directory
	}

	inFile.seekg(0, ios::end); // Seek to end of file
	fileSize = (int)inFile.tellg(); //	Get current position in file - The End, this givesus the total file size
	tempTextureData = new char[fileSize]; // Create a new array to store the data (of the correct size)
	inFile.seekg(0, ios::beg); // Seek back to beginning of the file.
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

	mTextureIDs.push_back(ID);

	return ID;
}

void Texture2D::DeleteTexture(GLuint TexID)
{
	glDeleteTextures(1, &TexID);
	for (int i = 0; i <  mTextureIDs.size(); i++)
	{
		if (mTextureIDs[i] = TexID)
			mTextureIDs.erase(mTextureIDs.end() + i);
	}
}

void Texture2D::DeleteAllTextures()
{
	for (GLuint TexID : mTextureIDs)
	{
		glDeleteTextures(1, &TexID);
	}

	mTextureIDs.clear();
}
