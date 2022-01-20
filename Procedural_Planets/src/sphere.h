#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "geometry_sphere.h"

class Sphere
{
public:
	Sphere(float radius, int sub);
	void draw();

	void translate(glm::vec3 v) { model = glm::translate(model, v); }
	void scale(glm::vec3 v) { model = glm::scale(model, v); }
	void rotate(float angle, glm::vec3 v) { model = glm::rotate(model, angle, v); }
	void reset() { model = glm::mat4(1.0f); }

	const glm::mat4 getModelMatrix() { return model; }

private:

	GeometrySphere mesh;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO;

	glm::mat4 model;

};
