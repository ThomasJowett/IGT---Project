#include "SpawnManager.h"
#include "RandomGenerator.h"

void SpawnManager::SpawnGameObjects(std::unordered_map<std::string, SpawnRoom> spawnRooms, GameScreen* screen)
{
	//GameScreen* screen = GameScreenManager::GetInstance()->GetCurrentScreen();

	for (auto spawnRoom : spawnRooms)
	{
		float minX, maxX, minY, maxY;

		minX = spawnRoom.second.position.x;
		maxX = minX + spawnRoom.second.width;
		maxY = -spawnRoom.second.position.y;
		minY = maxY - spawnRoom.second.height;
		

		for (std::pair<Prefab * const, int> prefab : spawnRoom.second.prefabs)
		{
			for (int i = 0; i < prefab.second; i++)
			{
				GameObject* gameObject = new GameObject(*prefab.first->GetPrefab());

				screen->AddGameObject(gameObject);
				gameObject->GetTransform()->mPosition = Vector3D(Random::FloatInRange(minX, maxX), Random::FloatInRange(minY, maxY), 0);
			}
		}
	}
}
