// Base on the CubeSphere of Song Ho
// Source : http://www.songho.ca/opengl/gl_sphere.html

#pragma once

#include <vector>
#include <map>
#include <cmath>

class Sphere
{
public:
	Sphere(float radius = 1.0f, int division = 3);

	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

	unsigned int getVertexSize() const { return (unsigned int)indices.size(); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }

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


	float radius;
	int subdivision;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int vertexCountPerRow;
	unsigned int vertexCountPerFace;

	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO;

};
