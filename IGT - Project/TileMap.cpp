#include "TileMap.h"
#include "Texture2D.h"
#include "TinyXML2.h"
#include "Settings.h"
#include <iostream>
#include <math.h>

TileMap::TileMap()
{
}

TileMap::TileMap(const char* mapfilename, const char* tilesetfilename, const char* tilesetCollisionfilename)
	:GameObject("TileMap", new Transform())
{
	LoadMap(mapfilename);
	LoadTileSet(tilesetCollisionfilename, tilesetfilename);

	RedrawMap();
}


TileMap::~TileMap()
{
	//Delete all elements off the array
	for (unsigned int i = 0; i < mTilesHigh; i++)
	{
		delete[] mTiles[i];
	}
	delete[] mTiles;
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

	mMesh->Draw();

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
		

		//Allocate the memory for the array
		mTiles = new int*[mTilesWide];
		for (unsigned int i = 0; i < mTilesHigh; i++)
		{
			mTiles[i] = new int[mTilesWide];
		}

		pLayer = pRoot->FirstChildElement("layer");
		
		while (pLayer)
		{
			const char* name = pLayer->Attribute("name");

			if (std::strcmp(name, "Terrain") == 0)
			{
				//load into tiles
				tinyxml2::XMLElement* pTile;
				pTile = pLayer->FirstChildElement("tile");

				while (pTile)
				{
					mTiles[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = atoi(pTile->Attribute("tile"));

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
		return true;
	}
	else
	{
		std::cerr << "ERROR loading " << filename << " Code: " << doc.LoadFile(filename);
		return false;
	}

	
}

bool TileMap::LoadTileSet(const char * tileSetfilename, const char* spritefilename)
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLError errorCode = doc.LoadFile(tileSetfilename);

	if (errorCode == 0)
	{
		tinyxml2::XMLElement* pRoot;
		pRoot = doc.FirstChildElement("tileset");
		
		mTileSetWidth = atoi(pRoot->Attribute("tileswide"));
		mTileSetHeight = atoi(pRoot->Attribute("tileshigh"));
		mTileWidth = atoi(pRoot->Attribute("tilewidth"));
		mTileHeight = atoi(pRoot->Attribute("tileheight"));
		
		//Allocate the memory for the array
		mCollision = new bool*[mTileSetWidth];
		for (unsigned int i = 0; i < mTileSetHeight; i++)
		{
			mCollision[i] = new bool[mTileSetWidth];
		}
		
		tinyxml2::XMLElement* pTile;
		pTile = pRoot->FirstChildElement("tile");
		
		while (pTile)
		{
			//mCollision[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = atoi(pTile->Attribute("collision"));
		
			pTile = pTile->NextSiblingElement("tile");
		}

		mTextureID = Texture2D::LoadTexture2D(spritefilename);
		
		return true;
	}
	else
	{
		std::cerr << "ERROR loading " << tileSetfilename << " Code: "<< errorCode;
	}

	return false;
}

//Creates a grid mesh with the map correctly laid out on it
void TileMap::RedrawMap()
{
	IndexedModel model;

	int indicesIndex = 0;

	//Vertices
	for (int i = 0;  i < mTilesWide; i++)
	{
		float PosX = i * mTileWidth;

		for (int j = 0; j < mTilesHigh; ++j)
		{
			float PosY = j * -mTileHeight;
			model.positions.push_back(Vector3D(PosX, PosY, 0));
			model.texCoords.push_back(TextureCoordinatesAtIndex(0, mTiles[i][j]));

			PosX += mTileWidth;
			model.positions.push_back(Vector3D(PosX, PosY, 0));
			model.texCoords.push_back(TextureCoordinatesAtIndex(1, mTiles[i][j]));

			PosY += mTileHeight;
			model.positions.push_back(Vector3D(PosX, PosY, 0));
			model.texCoords.push_back(TextureCoordinatesAtIndex(2, mTiles[i][j]));

			PosX -= mTileWidth;
			model.positions.push_back(Vector3D(PosX, PosY, 0));
			model.texCoords.push_back(TextureCoordinatesAtIndex(3, mTiles[i][j]));

			model.indices.push_back(indicesIndex);
			model.indices.push_back(indicesIndex + 1);
			model.indices.push_back(indicesIndex + 2);
			model.indices.push_back(indicesIndex);
			model.indices.push_back(indicesIndex + 2);
			model.indices.push_back(indicesIndex + 3);

			indicesIndex += 4;
		}
	}

	mMesh = new Mesh(model);
}

//returns the index of the tile set at the position
int TileMap::GetTileAt(Vector2D position)
{
	position.x = position.x / mTileHeight;
	position.y = position.y / mTileWidth;

	unsigned int x = (unsigned int)position.x;// -(mTileWidth / 2);
	unsigned int y = ((unsigned int)position.y * -1);// -(mTileHeight / 2);

	//if position is indside tilemap bounds
	if (x < GetTransform()->mPosition.x + mTilesWide && y < GetTransform()->mPosition.y + mTilesHigh 
		&& x > GetTransform()->mPosition.x && y > GetTransform()->mPosition.y)
	{
		return mTiles[x][y];
	}
	return -1;
}

bool TileMap::IntersectsCollider(Collider * otherCollider, Vector2D & normal, float & penetrationDepth)
{
	if (otherCollider->mType == BOX2D)
	{
		Box2D * otherBox = dynamic_cast<Box2D*>(otherCollider);


	}
	return false;
}

//returns the texture coordinates of a tile index at the 4 corners of the quad
//used for creating the mesh
Vector2D TileMap::TextureCoordinatesAtIndex(int index, int tile)
{
	Vector2D position;
	if (tile == 10)
	{
		float test = 2 + 3;
	}

	long double intpart;

	float oneTileWide = 1 / (float)mTileSetWidth;
	float oneTileHeigh = 1 / (float)mTileSetHeight;

	position.x = modf(oneTileWide * tile, &intpart);

	if (index == 1 || index == 2)
	{
		position.x += oneTileWide;
		if (position.x > 1.0f)
			position.x = 1.0f;
	}

	position.y = modf(oneTileHeigh * (tile / mTileSetWidth), &intpart);

	if (index == 0 || index == 1)
	{
		position.y += oneTileHeigh;
		if (position.x > 1.0f)
			position.x = 1.0f;
	}

	return position;
}
