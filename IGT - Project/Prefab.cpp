#include "Prefab.h"
#include "TinyXML2.h"

std::vector<GameObject*> Prefab::LoadPrefab(std::string filename)
{
	std::vector<GameObject*> returnObjects;

	tinyxml2::XMLDocument doc;

	std::string prefix = "Prefabs/";

	if (doc.LoadFile((prefix + filename).c_str()) == 0)
	{

	}

	return returnObjects;
}
