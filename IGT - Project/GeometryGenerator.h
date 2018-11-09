#pragma once
#include "Commons.h"
#include "Mesh.h"

namespace Geometry
{
	Mesh* CreateQuad(float width, float height);
	std::vector<Mesh*> CreateQuadList(float singleWidth, float singleHeight, int quadsTall, int quadsWide);

	Mesh* CreateFullscreenQuad();
}