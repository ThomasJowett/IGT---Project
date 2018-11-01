#include "TileMap.h"
#include "Texture2D.h"
#include "TinyXML2.h"
#include "Settings.h"
#include <iostream>
#include <math.h>

TileMap::TileMap()
{
}

TileMap::TileMap(const char* mapfilename, const char* pallettefilename)
	:GameObject("TileMap", new Transform())
{
	LoadMap(mapfilename);

	mTextureID = Texture2D::LoadTexture2D(pallettefilename);

	RedrawMap();

	GameObject::SetLayer(BACKGROUND);
}


TileMap::~TileMap()
{
	//Delete all elements off the array
	for (unsigned int i = 0; i < mTilesHigh; i++)
	{
		delete[] mBackgroundTiles[i];
	}
	delete[] mBackgroundTiles;
}

void TileMap::Update(float deltatime)
{
	GameObject::Update(deltatime);

	Vector2D vector = Vector2D(Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.x, Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.y);

	//TODO: if camera distance from the center is too far from the center re draw the map
}

void TileMap::Render(Shader * shader)
{
	GameObject::Render(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	mBackGround->Draw();
	mForeground->Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool TileMap::LoadMap(const char * filename)
{
	tinyxml2::XMLDocument doc;

	if (doc.LoadFile(filename) == 0)
	{
		tinyxml2::XMLElement* pRoot;
		tinyxml2::XMLElement* pLayer;
		pRoot = doc.FirstChildElement("tilemap");

		mTilesWide = atoi(pRoot->Attribute("tileswide"));
		mTilesHigh = atoi(pRoot->Attribute("tileshigh"));
		mTileWidth = atoi(pRoot->Attribute("tilewidth"));
		mTileHeight = atoi(pRoot->Attribute("tileheight"));
		mPaletteWidth = atoi(pRoot->Attribute("palettewidth"));
		mPaletteHeight = atoi(pRoot->Attribute("paletteheight"));
		

		//Allocate the memory for the array
		mBackgroundTiles = new int*[mTilesWide];
		mCollision = new bool*[mTilesWide];
		mForegroundTiles = new int*[mTilesWide];
		for (unsigned int i = 0; i < mTilesHigh; i++)
		{
			mBackgroundTiles[i] = new int[mTilesWide];
			mForegroundTiles[i] = new int[mTilesWide];
			mCollision[i] = new bool[mTilesWide];
		}

		pLayer = pRoot->FirstChildElement("layer");
		
		while (pLayer)
		{
			const char* name = pLayer->Attribute("name");

			if (std::strcmp(name, "Background") == 0)
			{
				//load into tiles
				tinyxml2::XMLElement* pTile;
				pTile = pLayer->FirstChildElement("tile");

				while (pTile)
				{
					if (pTile->Attribute("tile") != "-1")
					{
						mBackgroundTiles[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = atoi(pTile->Attribute("tile"));
					}
					pTile = pTile->NextSiblingElement("tile");
				}

			}
			else if (std::strcmp(name, "Collision") == 0)
			{
				//load into collision
				tinyxml2::XMLElement* pTile;
				pTile = pLayer->FirstChildElement("tile");

				while (pTile)
				{
					bool collision;
					if (atoi(pTile->Attribute("tile")) == -1)
						collision = false;
					else if (atoi(pTile->Attribute("tile")) == 1)
						collision = true;

					mCollision[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = collision;


					pTile = pTile->NextSiblingElement("tile");
				}
			}
			else if (std::strcmp(name, "Foreground"))
			{
				tinyxml2::XMLElement* pTile;
				pTile = pLayer->FirstChildElement("tile");

				while (pTile)
				{
					if (pTile->Attribute("tile") != "-1")
					{
						mForegroundTiles[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = atoi(pTile->Attribute("tile"));
					}
					pTile = pTile->NextSiblingElement("tile");
				}
			}
			else if (std::strcmp(name, "Objects") == 0)
			{
				tinyxml2::XMLElement* pObject;
				pObject = pLayer->FirstChildElement("SpawnLocation");


				//load objects
			}
			else if (std::strcmp(name, "Events") == 0)
			{
				//load events
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}

		mCollider = new Box2D(this, mTileWidth, mTileHeight, Vector2D(0, 0));
		return true;
	}
	else
	{
		std::cerr << "ERROR loading " << filename << " Code: " << doc.LoadFile(filename);
		return false;
	}
}

//Creates a grid mesh with the map correctly laid out on it
void TileMap::RedrawMap()
{
	IndexedModel model;

	int indicesIndex = 0;

	float backgroundDepth = Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.z 
		- Settings::GetInstance()->GetCamera()->GetFarDepth() + 0.1f;
	float foregroundDepth = Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.z - 0.1f;

	//Vertices
	for (int i = 0;  i < mTilesWide; i++)
	{
		float PosX = i * mTileWidth;

		for (int j = 0; j < mTilesHigh; ++j)
		{

			float PosY = (j * -mTileHeight) -mTileHeight;
			model.positions.push_back(Vector3D(PosX, PosY, backgroundDepth));
			model.texCoords.push_back(TextureCoordinatesAtIndex(0, mBackgroundTiles[i][j]));

			PosX += mTileWidth;
			model.positions.push_back(Vector3D(PosX, PosY, backgroundDepth));
			model.texCoords.push_back(TextureCoordinatesAtIndex(1, mBackgroundTiles[i][j]));

			PosY += mTileHeight;
			model.positions.push_back(Vector3D(PosX, PosY, backgroundDepth));
			model.texCoords.push_back(TextureCoordinatesAtIndex(2, mBackgroundTiles[i][j]));

			PosX -= mTileWidth;
			model.positions.push_back(Vector3D(PosX, PosY, backgroundDepth));
			model.texCoords.push_back(TextureCoordinatesAtIndex(3, mBackgroundTiles[i][j]));

			model.indices.push_back(indicesIndex);
			model.indices.push_back(indicesIndex + 1);
			model.indices.push_back(indicesIndex + 2);
			model.indices.push_back(indicesIndex);
			model.indices.push_back(indicesIndex + 2);
			model.indices.push_back(indicesIndex + 3);

			indicesIndex += 4;
		}
	}

	mBackGround = new Mesh(model);

	IndexedModel modelForeground;
	indicesIndex = 0;

	for (int i = 0; i < mTilesWide; i++)
	{
		float PosX = i * mTileWidth;

		for (int j = 0; j < mTilesHigh; ++j)
		{
			if (mForegroundTiles[i][j])
			{
				float PosY = (j * -mTileHeight) - mTileHeight;
				modelForeground.positions.push_back(Vector3D(PosX, PosY, foregroundDepth));
				modelForeground.texCoords.push_back(TextureCoordinatesAtIndex(0, mForegroundTiles[i][j]));

				PosX += mTileWidth;
				modelForeground.positions.push_back(Vector3D(PosX, PosY, foregroundDepth));
				modelForeground.texCoords.push_back(TextureCoordinatesAtIndex(1, mForegroundTiles[i][j]));

				PosY += mTileHeight;
				modelForeground.positions.push_back(Vector3D(PosX, PosY, foregroundDepth));
				modelForeground.texCoords.push_back(TextureCoordinatesAtIndex(2, mForegroundTiles[i][j]));

				PosX -= mTileWidth;
				modelForeground.positions.push_back(Vector3D(PosX, PosY, foregroundDepth));
				modelForeground.texCoords.push_back(TextureCoordinatesAtIndex(3, mForegroundTiles[i][j]));

				modelForeground.indices.push_back(indicesIndex);
				modelForeground.indices.push_back(indicesIndex + 1);
				modelForeground.indices.push_back(indicesIndex + 2);
				modelForeground.indices.push_back(indicesIndex);
				modelForeground.indices.push_back(indicesIndex + 2);
				modelForeground.indices.push_back(indicesIndex + 3);

				indicesIndex += 4;
			}
		}
	}
	
	mForeground = new Mesh(modelForeground);
}

//returns the index of the tile set at the position
int TileMap::GetTileAt(Vector2D position)
{
	unsigned int x;
	unsigned int y;

	if(PositionToTileIndex(position, x, y))
		return mBackgroundTiles[x][y];

	return -1;
}

bool TileMap::GetCollisionAt(Vector2D position)
{
	unsigned int x;
	unsigned int y;

	if(PositionToTileIndex(position, x, y))
		return mCollision[x][y];

	return false;
}

void TileMap::SetColliderPosition(Vector2D position)
{
	unsigned int x;
	unsigned int y;

	if (PositionToTileIndex(position, x, y))
	{
		if (mCollision[x][y])
		{
			mCollider->SetOffset(Vector2D((x * mTileWidth) + (mTileWidth / 2), ((float)y * -mTileHeight) - (mTileWidth / 2)));
		}
	}
}

//returns the texture coordinates of a tile index at the 4 corners of the quad
//used for creating the mesh
Vector2D TileMap::TextureCoordinatesAtIndex(int index, int tile)
{
	Vector2D position;

	long double intpart;

	float oneTileWide = 1 / (float)mPaletteWidth;
	float oneTileHeigh = 1 / (float)mPaletteHeight;

	position.x = modf(oneTileWide * tile, &intpart);

	if (index == 1 || index == 2)
	{
		position.x += oneTileWide;
		if (position.x > 1.0f)
			position.x = 1.0f;
	}

	position.y = modf(oneTileHeigh * (tile / mPaletteWidth), &intpart);

	if (index == 0 || index == 1)
	{
		position.y += oneTileHeigh;
		if (position.x > 1.0f)
			position.x = 1.0f;
	}

	//tiny shift in texture coordinates to prevent overscan
	if(index == 0 || index == 3)
		position.x += 0.0001f;
	else
		position.x -= 0.0001f;

	return position;
}

//converts a position in the world to x and Y position on the tileMap
bool TileMap::PositionToTileIndex(Vector2D position, unsigned int &X, unsigned int &Y)
{
	Transform * worldTransform = GetWorldTransform();
	position -= Vector2D(worldTransform->mPosition.x, worldTransform->mPosition.y);

	position.x = position.x / mTileWidth;
	position.y = position.y / mTileHeight;

	X = floor(position.x);
	Y = floor(position.y * -1);

	return (X < mTilesWide && Y < mTilesHigh);
}
