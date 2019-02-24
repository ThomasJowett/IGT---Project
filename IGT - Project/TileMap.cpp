#include "TileMap.h"
#include "Texture2D.h"
#include "TinyXML2.h"
#include "Settings.h"
#include <iostream>
#include <math.h>
#include "Commons.h"

TileMap::TileMap()
{
}

TileMap::TileMap(const char* mapfilename)
	:GameObject("TileMap", new Transform())
{
	mIsActive = LoadMap(mapfilename);

	if(mIsActive)
		RedrawMap();

	GameObject::SetLayer(BACKGROUND);
}

TileMap::TileMap(int ** backgroundTiles, int ** foregroundTiles, bool ** collision, int tileHeight, int tileWidth, const char* tileSetName)
{
	mBackgroundTiles = backgroundTiles;
	mForegroundTiles = foregroundTiles;
	mCollision = collision;

	mTileHeight = tileHeight;
	mTileWidth = tileWidth;

	mTileSet = new TileSet(tileSetName);

	mTilesWide = sizeof(mBackgroundTiles[0]);

	RedrawMap();

	GameObject::SetLayer(BACKGROUND);
}


TileMap::~TileMap()
{
	//Delete all elements off the array
	for (unsigned int i = 0; i < mTilesHigh; i++)
	{
		if (mBackgroundTiles) delete[] mBackgroundTiles[i];
		if (mForegroundTiles) delete[] mForegroundTiles[i];
		if (mCollision) delete[] mCollision[i];
	
	}
	if (mBackgroundTiles) delete[] mBackgroundTiles;
	if (mForegroundTiles) delete[] mForegroundTiles;
	if (mCollision) delete[] mCollision;

	delete mCollider;
	delete mForeground;
	delete mBackGround;

	if (mTileSet) delete mTileSet;
}

void TileMap::Update(float deltatime)
{
	GameObject::Update(deltatime);

	//Vector2D vector = Vector2D(Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.x, Settings::GetInstance()->GetCamera()->GetTransform()->mPosition.y);

	//TODO: if camera distance from the center is too far from the center re draw the map
}

void TileMap::Render(Shader * shader)
{
	GameObject::Render(shader);

	if (mIsActive)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTileSet->GetTextureID());

		shader->Updatefloat4(1.0f, 1.0f, 1.0f, 1.0f);
		mBackGround->Draw();
		mForeground->Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//Loads a map from a .tmx file
bool TileMap::LoadMap(std::string filename)
{
	tinyxml2::XMLDocument doc;

	std::string mapPrefix = "Maps/";

	if (doc.LoadFile((mapPrefix + filename).c_str()) == 0)
	{
		tinyxml2::XMLElement* pRoot;
		tinyxml2::XMLElement* pLayer;

		pRoot = doc.FirstChildElement("map");

		//Map Attributes-------------------------------------------------------
		mTilesWide = atoi(pRoot->Attribute("width"));
		mTilesHigh = atoi(pRoot->Attribute("height"));
		mTileWidth = atoi(pRoot->Attribute("tilewidth"));
		mTileHeight = atoi(pRoot->Attribute("tileheight"));

		std::string backgroundcolor = pRoot->Attribute("backgroundcolor");

		backgroundcolor.erase(0, 1);
		unsigned long value  = std::stoul(backgroundcolor, nullptr, 16);

		SDL_Colour colour;
		colour.a = (value >> 24) & 0xff;
		colour.r = ((value >> 16) & 0xff);
		colour.g = ((value >> 8) & 0xff);
		colour.b = ((value >> 0) & 0xff);

		glClearColor((float)colour.r/256.0f, (float)colour.g/255.0f, (float)colour.b/255.0f, (float)colour.a/255.0f);

		//Load Tileset----------------------------------------------------------
		pLayer = pRoot->FirstChildElement("tileset");
		std::string tsxPath = pLayer->Attribute("source");

		mTileSet = new TileSet((mapPrefix + tsxPath).c_str());

		if (mTileSet->GetPalatteHeight() == -1)
		{
			return false;
		}

		//Allocate the memory for the array------------------------------------
		mBackgroundTiles = new int*[mTilesWide];
		mCollision = new bool*[mTilesWide];
		mForegroundTiles = new int*[mTilesWide];
		for (unsigned int i = 0; i < mTilesHigh; i++)
		{
			mBackgroundTiles[i] = new int[mTilesWide];
			mForegroundTiles[i] = new int[mTilesWide];
			mCollision[i] = new bool[mTilesWide];
		}

		//Load tile data-------------------------------------------------------

		pLayer = pRoot->FirstChildElement("layer");

		while (pLayer)
		{
			const char* name = pLayer->Attribute("name");

			tinyxml2::XMLElement* pData;

			pData = pLayer->FirstChildElement("data");

			const char* encoding = pData->Attribute("encoding");
			std::vector<std::string> SeperatedData;

			if (encoding)
			{
				if (!strcmp(encoding, "csv"))
				{
					std::string pDataNode = pData->GetText();

					std::vector<std::string> SeperatedData = Util::SplitString(pDataNode, ',');


					if (std::strcmp(name, "Background") == 0)
					{
						for (unsigned int i = 0; i < mTilesHigh; i++)
						{
							for (unsigned int j = 0; j < mTilesWide; j++)
							{
								int Index = atoi(SeperatedData[(i*mTilesWide) + j].c_str());
								mBackgroundTiles[j][i] = Index - 1;
							}
						}
					}
					else if (std::strcmp(name, "Collision") == 0)
					{
						for (unsigned int i = 0; i < mTilesHigh; i++)
						{
							for (unsigned int j = 0; j < mTilesWide; j++)
							{
								mCollision[j][i] = atoi(SeperatedData[(i*mTilesWide) + j].c_str());
							}
						}
					}
					else if (std::strcmp(name, "Foreground") == 0)
					{
						for (unsigned int i = 0; i < mTilesHigh; i++)
						{
							for (unsigned int j = 0; j < mTilesWide; j++)
							{
								int Index = atoi(SeperatedData[(i*mTilesWide) + j].c_str());
								mForegroundTiles[j][i] = Index - 1;
							}
						}
					}
				}
			}
			else
			{
				std::cerr << "ERROR loading " << filename << " Incorrect Data Format\n";
				return false;
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}

		//objects----------------------------------------------------------------
		pLayer = pRoot->FirstChildElement("objectgroup");

		while (pLayer)
		{
			const char* name = pLayer->Attribute("name");

			tinyxml2::XMLElement* pObject;

			pObject = pLayer->FirstChildElement("object");

			while (pObject)
			{
				const char* type = pObject->Attribute("type");

				if (type)
				{
					//Player Starts -----------------------------------------------
					if (std::strcmp(type, "Player Start") == 0)
					{
						Vector2D spawnPoint = { (float)atof(pObject->Attribute("x")), (float)atof(pObject->Attribute("y")) };

						spawnPoint.y *= -1;

						mPlayerStarts.push_back(spawnPoint);
					}

					//SpawnRooms -------------------------------------------------
					else if (std::strcmp(type, "SpawnRoom") == 0)
					{
						Vector2D position = { (float)atof(pObject->Attribute("x")), (float)atof(pObject->Attribute("y")) };
						float width = (float)atof(pObject->Attribute("width"));
						float height = (float)atof(pObject->Attribute("height"));

						if (pObject->FirstChildElement("properties"))
						{
							tinyxml2::XMLElement* pProperty = pObject->FirstChildElement("properties")->FirstChildElement("property");

							std::unordered_map<Prefab*, int> prefabList;

							while (pProperty)
							{
								const char* name = pProperty->Attribute("name");

								int value = atoi(pProperty->Attribute("value"));

								Prefab* prefab = Factory<Prefab>::CreateInstance(name);

								if (prefab)
									prefabList[prefab] = value;

								pProperty = pProperty->NextSiblingElement("property");
							}

							SpawnRoom room = { position,width, height, prefabList };

							mSpawnRooms[pObject->Attribute("name")] = room;
						}
					}
				}

				pObject = pObject->NextSiblingElement("object");
			}

			pLayer = pLayer->NextSiblingElement("objectgroup");
		}

		//create the collider----------------------------------------------------
		mCollider = new Box2D(this, (float)mTileWidth, (float)mTileHeight, Vector2D(0, 0));
		return true;
	}
	else
	{
		std::cerr << "ERROR loading " << filename << " Code: " << doc.LoadFile((mapPrefix + filename).c_str());
		return false;
	}
}

//Creates a grid mesh with the map correctly laid out on it
void TileMap::RedrawMap()
{
	IndexedModel model;

	int indicesIndex = 0;

	float backgroundDepth = -99.9f;

	float foregroundDepth = -0.1f;

	//Vertices
	for (int i = 0;  i < mTilesWide; i++)
	{
		float PosX = (float)(i * mTileWidth);

		for (int j = 0; j < mTilesHigh; ++j)
		{
			if (mBackgroundTiles[i][j] != -1)
			{
				float PosY = (float)((j * -mTileHeight) - mTileHeight);
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
	}

	mBackGround = new Mesh(model);

	IndexedModel modelForeground;
	indicesIndex = 0;

	for (int i = 0; i < mTilesWide; i++)
	{
		float PosX = (float)(i * mTileWidth);

		for (int j = 0; j < mTilesHigh; ++j)
		{
			if (mForegroundTiles[i][j] != -1)
			{
				float PosY = (float)((j * -mTileHeight) - mTileHeight);
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

//returns if the position has collision on it
bool TileMap::GetCollisionAt(Vector2D position)
{
	unsigned int x;
	unsigned int y;

	if(PositionToTileIndex(position, x, y))
		return mCollision[x][y];

	return false;
}

//return if the tile has collision on it
bool TileMap::GetCollisionAt(int X, int Y)
{
	if (X >= 0 && Y >= 0 && X <= mTilesWide && mTilesHigh)
	{
		return mCollision[X][Y];
	}
	else
	{
		return false;
	}
}

//Sets the collision  box to the tile the position is on
int TileMap::SetColliderPosition(Vector2D position)
{
	unsigned int x;
	unsigned int y;

	if (PositionToTileIndex(position, x, y))
	{
		if (mCollision[x][y])
		{
			mCollider->SetOffset(Vector2D(((float)x * mTileWidth) + (mTileWidth / 2), ((float)y * -mTileHeight) - (mTileWidth / 2)));
		}
	}

	return x * y;
}

//returns the texture coordinates of a tile index at the 4 corners of the quad
//used for creating the mesh
Vector2D TileMap::TextureCoordinatesAtIndex(int index, int tile)
{
	Vector2D position;

	long double intpart;

	float oneTileWide = 1 / (float)mTileSet->GetPaletteWidth();
	float oneTileHeigh = 1 / (float)mTileSet->GetPalatteHeight();

	position.x = (float)modf(oneTileWide * tile, &intpart);

	if (index == 1 || index == 2)
	{
		position.x += oneTileWide;
		if (position.x > 1.0f)
			position.x = 1.0f;
	}

	position.y = (float)modf(oneTileHeigh * (tile / mTileSet->GetPaletteWidth()), &intpart);

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

	X = (unsigned int)floor(position.x);
	Y = (unsigned int)floor(position.y * -1);

	return (X < mTilesWide && Y < mTilesHigh);
}

//converts a tile map index to a position in the centre of that tile
bool TileMap::TileIndexToPosition(unsigned int X, unsigned int Y, Vector2D& position)
{
	position.x = (float)(X * mTileWidth + (mTileWidth / 2));
	position.y = (float)(((int)Y * mTileHeight + (float)(mTileHeight / 2)) * -1);

	Transform * worldTransform = GetWorldTransform();
	position += Vector2D(worldTransform->mPosition.x, worldTransform->mPosition.y);

	return (X < mTilesWide && Y < mTilesHigh);
}

TileSet::TileSet(const char * filename)
{
	//Loads the tileset from a .tsx file
	tinyxml2::XMLDocument doc;

	std::string mapPrefix = "Maps/";

	if (doc.LoadFile(filename) == 0)
	{
		tinyxml2::XMLElement* pRoot;

		pRoot = doc.FirstChildElement("tileset");

		mPaletteWidth = atoi(pRoot->Attribute("columns"));

		mPaletteHeight = atoi(pRoot->Attribute("tilecount")) / mPaletteWidth;

		tinyxml2::XMLElement* pImage;

		pImage = pRoot->FirstChildElement("image");

		std::string imagefilepath = mapPrefix + pImage->Attribute("source");

		mTextureID = Texture2D::GetTexture2D(imagefilepath.c_str());
	}
	else
	{
		mPaletteWidth = -1;
		mPaletteHeight = -1;

		mTextureID = Texture2D::GetTexture2D("Images/NULL.png");
	}
}

TileSet::~TileSet()
{
}
