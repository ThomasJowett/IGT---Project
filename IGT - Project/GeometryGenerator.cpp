#include "GeometryGenerator.h"

Mesh * Geometry::CreateQuad(float width, float height)
{
	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	float halfHeight = height / 2;
	float halfWidth = width / 2;

	Vertex vertices[] =
	{
		Vertex(Vector3D(-halfWidth,-halfHeight,0), Vector2D(0,1)),
		Vertex(Vector3D(halfWidth,-halfHeight,0), Vector2D(1, 1)),
		Vertex(Vector3D(halfWidth,halfHeight,0), Vector2D(1,0)),
		Vertex(Vector3D(-halfWidth,halfHeight,0), Vector2D(0,0))
	};

	return new Mesh(vertices, 4, indices, 6);
}

std::vector<Mesh*> Geometry::CreateQuadList(float singleWidth, float singleHeight, int quadsTall, int quadsWide)
{
	std::vector<Mesh*> meshes;

	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	float halfHeight = singleHeight / 2;
	float halfWidth = singleWidth / 2;
	float inverseTall = (float)1 / quadsTall;
	float inverseWide = (float)1 / quadsWide;

	for (int i = 0; i < quadsTall; i++)
	{
		for (int j = 0; j < quadsWide; j++)
		{
			Vertex vertices[] =
			{

				Vertex(Vector3D(-halfWidth,-halfHeight,0), Vector2D(j*inverseWide, (i + 1) * inverseTall)),
				Vertex(Vector3D(halfWidth,-halfHeight,0), Vector2D((j + 1) * inverseWide, (i + 1) * inverseTall)),
				Vertex(Vector3D(halfWidth,halfHeight,0), Vector2D((j + 1) * inverseWide,i * inverseTall)),
				Vertex(Vector3D(-halfWidth,halfHeight,0), Vector2D(j*inverseWide,i * inverseTall))
			};

			Mesh* mesh = new Mesh(vertices, 4, indices, 6);
			meshes.push_back(mesh);
		}
	}
	return meshes;
}

Mesh * Geometry::CreateFullscreenQuad()
{
	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	Vertex vertices[] =
	{
		Vertex(Vector3D(-1.0f,-1.0f,0), Vector2D(0,0)),
		Vertex(Vector3D(1.0f,-1.0f,0), Vector2D(1, 0)),
		Vertex(Vector3D(1.0f,1.0f,0), Vector2D(1,1)),
		Vertex(Vector3D(-1.0f,1.0f,0), Vector2D(0,1))
	};

	return new Mesh(vertices, 4, indices, 6);
}
