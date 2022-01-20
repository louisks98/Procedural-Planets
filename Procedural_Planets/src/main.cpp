#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "sphere.h"
#include "texture.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// windows initialization
	GLFWwindow* window = glfwCreateWindow(800, 600, "Procedural Planet", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader shader{ "shaders/perlin_noise_vs.glsl", "shaders/fragment.glsl" };
	Shader light_shader{ "shaders/perlin_noise_vs.glsl", "shaders/lightingShader_fs.glsl" };
	Sphere sphere{ 1.0f, 5 };
	float color[] = { 50.0f / 255, 168.0f / 255, 82.0f / 255 };
	
	//Texture texture1{ "textures/container.jpg" };
	//Texture texture2{ "textures/awesomeface.png" };
	//Texture earth{ "textures/earth.jpg" };

	//shader.use();
	//shader.setInt("texture1", 0);
	//shader.setInt("texture2", 1);

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	/*shader.use();
	shader.setMat4("model", sphere.getModelMatrix());
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("color", glm::vec3(50.0f / 255, 168.0f / 255, 82.0f / 255));*/

	light_shader.use();
	light_shader.setMat4("model", sphere.getModelMatrix());
	light_shader.setMat4("view", view);
	light_shader.setMat4("projection", projection);

	light_shader.use();
	light_shader.setVec3("direction", glm::vec3(-1.0f, -1.0f, -1.0f));
	light_shader.setVec3("diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	light_shader.setVec3("specular", glm::vec3(1.0f, 1.0f, 1.0f));
	light_shader.setVec3("ambient_light", glm::vec3(1.0f, 1.0f, 1.0f));

	//light_shader.setFloat("shininess", 0.22 * 128.0f);
	light_shader.setFloat("shininess", 128.0f);
	light_shader.setVec3("mat_diff", glm::vec3(0.1f, 0.35f, 0.1f));
	//light_shader.setVec3("mat_spec", glm::vec3(0.45f, 0.55f, 0.45f));
	light_shader.setVec3("mat_spec", glm::vec3(0.1f, 0.35f, 0.1f));
	//light_shader.setVec3("mat_ambient", glm::vec3());
	light_shader.setVec3("mat_ambient", glm::vec3(0.08f, 0.3f, 0.08f));

	light_shader.setVec3("cam_pos", glm::vec3(glm::vec3(0.0f, 0.0f, 3.0f)));

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//earth.use();

		//shader.use();
		sphere.reset();
		sphere.rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		sphere.rotate((float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//shader.setMat4("model", sphere.getModelMatrix());
		light_shader.setMat4("model", sphere.getModelMatrix());
		sphere.draw();

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		// 
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}