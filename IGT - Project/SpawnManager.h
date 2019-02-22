#pragma once

#include <string>
#include <unordered_map>
#include "TileMap.h"
#include "GameScreen.h"

namespace SpawnManager
{
	void SpawnGameObjects(std::unordered_map<std::string, SpawnRoom> spawnRoom, GameScreen* screen);
}