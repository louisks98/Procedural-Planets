// Base on the CubeSphere of Song Ho
// Source : http://www.songho.ca/opengl/gl_sphere.html

#pragma once


#include "geometry_sphere.h"

class Sphere
{
public:
	Sphere(float radius, int sub);
	void draw();

private:

	GeometrySphere mesh;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO;

};
