#pragma once

#include <vector>
#include <map>
#include <cmath>

class GeometrySphere
{
public:
	GeometrySphere() {};
	GeometrySphere(float radius, int sub);

	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

	unsigned int getVertexSize() const { return (unsigned int)indices.size(); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
	unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }

	const float* getVertices() const { return vertices.data(); }
	const unsigned int* getIndices() const { return indices.data(); }
	const float* getInterleavedVertices() const { return interleavedVertices.data(); }

	
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

	float radius;
	int subdivision;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> interleavedVertices;
	
	unsigned int vertexCountPerRow;
	unsigned int vertexCountPerFace;
};
