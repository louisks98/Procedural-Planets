#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

#include "shader.h"
#include "sphere.h"
#include "texture.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

struct Settings
{
	int numLayers = 1;
	float strength = 1;
	float baseRoughness = 1;
	float roughness = 2;
	float persistence = 0.5f;
	float minValue = 0;
	float* center = new float[3]{ 0.0f, 0.0f, 0.0f };
};

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

void updatePlanet(Settings settings, Shader& shader)
{
	shader.use();
	shader.setInt("numLayers", settings.numLayers);
	shader.setFloat("strength", settings.strength);
	shader.setFloat("baseRoughness", settings.baseRoughness);
	shader.setFloat("roughness", settings.roughness);
	shader.setFloat("persistence", settings.persistence);
	shader.setFloat("minValue", settings.minValue);
	shader.setVec3("center", glm::vec3(settings.center[0], settings.center[1], settings.center[3]));
}

void setLights(Shader& shader)
{
	shader.use();
	shader.setVec3("direction", glm::vec3(-1.0f, -1.0f, -1.0f));
	shader.setVec3("diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("ambient_light", glm::vec3(1.0f, 1.0f, 1.0f));

	//light_shader.setFloat("shininess", 0.22 * 128.0f);
	shader.setFloat("shininess", 128.0f);
	shader.setVec3("mat_diff", glm::vec3(0.1f, 0.35f, 0.1f));
	//light_shader.setVec3("mat_spec", glm::vec3(0.45f, 0.55f, 0.45f));
	shader.setVec3("mat_spec", glm::vec3(0.1f, 0.35f, 0.1f));
	//light_shader.setVec3("mat_ambient", glm::vec3());
	shader.setVec3("mat_ambient", glm::vec3(0.08f, 0.3f, 0.08f));

	shader.setVec3("cam_pos", glm::vec3(glm::vec3(0.0f, 0.0f, 3.0f)));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int windowWidth = 1280;
	int windowHeight = 720;

	// windows initialization
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Procedural Planet", NULL, NULL);
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

	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// UI initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");


	Shader shader{ "shaders/perlin_noise_vs.glsl", "shaders/fragment.glsl" };
	Shader light_shader{ "shaders/perlin_noise_vs.glsl", "shaders/lightingShader_fs.glsl" };
	Sphere sphere{ 1.0f, 5 };
	Settings settings;
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
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

	/*shader.use();
	shader.setMat4("model", sphere.getModelMatrix());
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("color", glm::vec3(50.0f / 255, 168.0f / 255, 82.0f / 255));*/

	light_shader.use();
	light_shader.setMat4("model", sphere.getModelMatrix());
	light_shader.setMat4("view", view);
	light_shader.setMat4("projection", projection);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		setLights(light_shader);
		updatePlanet(settings, light_shader);

		light_shader.use();
		sphere.reset();
		sphere.rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		sphere.rotate((float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		light_shader.setMat4("model", sphere.getModelMatrix());
		sphere.draw();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");
		ImGui::SliderInt("Number of layers", &settings.numLayers, 1, 10);
		ImGui::SliderFloat("Strength", &settings.strength, 0.0f, 10.0f);
		ImGui::SliderFloat("Base roughness", &settings.baseRoughness, 0.0f, 10.0f);
		ImGui::SliderFloat("Roughness", &settings.roughness, 0.0f, 10.0f);
		ImGui::SliderFloat("Persistence", &settings.persistence, 0.0f, 10.0f);
		ImGui::SliderFloat("Minimum value", &settings.minValue, 0.0f, 10.0f);
		ImGui::SliderFloat3("Center", settings.center, 1.0f, 10.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}