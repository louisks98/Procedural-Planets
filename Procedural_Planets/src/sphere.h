// Base on the CubeSphere of Song Ho
// Source : http://www.songho.ca/opengl/gl_sphere.html

#pragma once

#include <vector>
#include <map>
#include <cmath>

class Sphere
{
public:
	Sphere(float radius, int sectors, int stacks);

	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

	unsigned int getVertexSize() const { return (unsigned int)indices.size(); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
	unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }

	const std::vector<float> getVertices() { return vertices; }
	const std::vector<unsigned int> getIndices() { return indices; }

	void draw();

private:
	void buildSphere();
	std::vector<float> getUnitPositiveX(unsigned int pointsPerRow);
	float computeScaleForLength(const float v[3], float length);
	void addVertex(float x, float y, float z);
	void addVertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3]);
	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
	void addNormal(float nx, float ny, float nz);
	void addNormals(const float n1[3], const float n2[3], const float n3[3], const float n4[3]);
	void addTexCoord(float s, float t);
	void addTexCoords(const float t1[2], const float t2[2], const float t3[2], const float t4[2]);

	void buildInterleavedVertices();

	const int MIN_SECTOR_COUNT = 3;
	const int MIN_STACK_COUNT = 2;

	float radius;
	//int subdivision;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> interleavedVertices;

	unsigned int vertexCountPerRow;
	unsigned int vertexCountPerFace;

	int sectorCount;
	int stackCount;

	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO;

};
