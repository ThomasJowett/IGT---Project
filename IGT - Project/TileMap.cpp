#include "TileMap.h"
#include "Texture2D.h"
#include "TinyXML2.h"
#include "Settings.h"
#include <iostream>

TileMap::TileMap()
{
}

TileMap::TileMap(const char* mapfilename, const char* tilesetfilename)
	:GameObject("TileMap", new Transform())
{
	LoadMap(mapfilename);
	LoadTileSet("Maps/TestMapCollision.xml", tilesetfilename);

	mTileWidth = 16;
	mTileHeight = 16;
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

	//TODO: if camera distance from the center is too far from the center re draw the map
}

void TileMap::Render(Shader * shader)
{
	GetTransform()->UpdateWorldMatrix();
	shader->UpdateMatrixUniform(MODEL_U, GetTransform()->GetWorldMatrix(), true);

	for (int i = 0; i < mTilesWide-1; i++)
	{
		for (int j = 0; j < mTilesHigh-1; j++)
		{
			mTileSet->SetCurrentFrame(mTiles[i][j]);
			mTileSet->SetOffset(Vector2D(i*mTileWidth, j*-mTileHeight));
			mTileSet->Render(shader);
		}
	}
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
			else if (name == "Objects")
			{
				//load objects
			}
			else if (name == "Events")
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
	int width, height, tileWidth, tileheight;

	tinyxml2::XMLError errorCode = doc.LoadFile(tileSetfilename);

	if (errorCode == 0)
	{
		tinyxml2::XMLElement* pRoot;
		pRoot = doc.FirstChildElement("tileset");
		
		width = atoi(pRoot->Attribute("tileswide"));
		height = atoi(pRoot->Attribute("tileshigh"));
		tileWidth = atoi(pRoot->Attribute("tilewidth"));
		tileheight = atoi(pRoot->Attribute("tileheight"));
		
		//Allocate the memory for the array
		mCollision = new bool*[width];
		for (unsigned int i = 0; i < height; i++)
		{
			mCollision[i] = new bool[width];
		}
		
		tinyxml2::XMLElement* pTile;
		pTile = pRoot->FirstChildElement("tile");
		
		while (pTile)
		{
			//mCollision[atoi(pTile->Attribute("x"))][atoi(pTile->Attribute("y"))] = atoi(pTile->Attribute("collision"));
		
			pTile = pTile->NextSiblingElement("tile");
		}
		
		mTileSet = new Sprite(this, Texture2D::LoadTexture2D(spritefilename), tileWidth, tileheight, width, height);
		
		return true;
	}
	else
	{
		std::cerr << "ERROR loading " << tileSetfilename << " Code: "<< errorCode;
	}
	

	mTileSet = new Sprite(this, Texture2D::LoadTexture2D(spritefilename), 16, 16, 12, 11);

	return false;
}

//Creates a grid mesh with the map correctly laid out on it
void TileMap::RedrawMap()
{
	//TODO: make the map be drawn as a single mesh so that it does not have the wierd lines occassionally;
	float width = mTilesWide * mTileWidth;
	float height = mTilesHigh * mTileHeight;

	unsigned int tilecount = mTilesWide * mTilesHigh;

	IndexedModel model;

	model.positions.reserve(tilecount + 1);
	model.texCoords.reserve(tilecount * 4);
	//model.indices.reserve();

	//for (unsigned int i = 0; i < mTilesWide + 1; i++);
	//{
	//	float y = (height / 2) - (i * mTileHeight);
	//	for (unsigned int j = 0; j < mTilesHigh + 1; ++j)
	//	{
	//		float x = -width / 2 + j * mTileWidth;
	//		model.positions[i*(mTilesHigh + 1) + j] = Vector3D(x, y, 0);
	//
	//		model.positions[]
	//	}
	//}
}

int TileMap::GetTileAt(float x, float y)
{
	if (x < mTilesWide && y < mTilesHigh)
	{
		return mTiles[(int)x][(int)y];
	}
	return 0;
}
