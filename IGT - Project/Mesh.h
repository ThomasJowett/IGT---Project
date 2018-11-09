#pragma once
#include "Commons.h"

struct IndexedModel
{
public:
	std::vector<Vector3D> positions;
	std::vector<Vector2D> texCoords;
	std::vector<unsigned int> indices;

	void CalcNormals();
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(IndexedModel model);
	~Mesh();

	void Draw();

private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint mVertexArrayObject;
	GLuint mVertexArrayBuffers[NUM_BUFFERS];
	unsigned int mDrawCount;

	void InitMesh(const IndexedModel& model);
};