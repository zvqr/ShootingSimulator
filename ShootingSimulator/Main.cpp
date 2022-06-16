using namespace std;
#pragma comment(lib, "glfw3.lib")
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.hpp"
#include "stb_image.h"

const GLuint WIDTH = 1280, HEIGHT = 720; 

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1280 / 2.0;
float lastY = 720 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int readyShotint = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 70.0f)
		pitch = 70.0f;
	if (pitch < -70.0f)
		pitch = -70.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	readyShotint = 0;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		readyShotint = 1;
	}
}
struct Vertex
{
	float x, y, z;
	float s, t;
	Vertex(float x, float y, float z, float s, float t)
		:x(x), y(y), z(z), s(s), t(t) {}
};
class Drawable
{
protected:

	unsigned m_vao;

public:

	Drawable()
	{
		m_vao = 0;
	}
	~Drawable()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	virtual void CreateVAO() = 0;
	virtual void Draw(ShaderProgram& shader) = 0;
};
class Map : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
public:
	Map(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		//predna strana
		vertices.push_back(Vertex(-0.5f, -0.5f, -4.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -4.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -4.0f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -4.0f, 0.0f, 0.0f));
		//zaden kub lqva strana
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 1.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		//zaden kub dqsna strana
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 1.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		//zadna strana
		vertices.push_back(Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 1.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 1.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 1.0f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 1.0f, 0.0f, 0.0f));
		//lqv kub strana otpred
		vertices.push_back(Vertex(-2.0f, 0.5f, -0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(-2.0f, 0.5f, -0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-2.0f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 4.0f, 0.0f));
		//lqv kub strana otzad
		vertices.push_back(Vertex(-2.0f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(-2.0f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-2.0f, -0.5f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 4.0f, 0.0f));
		//lqva strana
		vertices.push_back(Vertex(-2.0f, 0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-2.0f, -0.5f, -0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(-2.0f, -0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-2.0f, -0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-2.0f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-2.0f, 0.5f, -0.5f, 0.0f, 0.0f));
		//dqsna strana
		vertices.push_back(Vertex(1.5f, 0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.5f, -0.5f, -0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(1.5f, -0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(1.5f, -0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(1.5f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(1.5f, 0.5f, -0.5f, 0.0f, 0.0f));
		//desen kub strana otzad
		vertices.push_back(Vertex(1.5f, 0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.5f, 0.5f, 0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(1.5f, -0.5f, 0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		//desen kub strana otpred
		vertices.push_back(Vertex(1.5f, 0.5f, -0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(1.5f, 0.5f, -0.5f, 4.0f, 4.0f));
		vertices.push_back(Vertex(1.5f, -0.5f, -0.5f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		//preden kub lqva strana
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -4.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		//preden kub dqsna strana
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -4.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 0.0f, 4.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f));

	}

	float XMax() const { return p.x + 50.f;}
	float XMin() const { return p.x - 50.f;}
	float YMax() const { return p.y + 5.f;}
	float YMin() const { return p.y - 5.f;}
	float ZMax() const { return p.z + 25.f;}
	float ZMin() const { return p.z - 100.f;}

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/w.jpg", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE20);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 20);
		glBindVertexArray(m_vao);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, p);
		model = glm::scale(model, glm::vec3(25, 10, 25));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
};
class crates : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
public:
	crates(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, -0.5f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,  0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,   1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f,  0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,  0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f,  0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, -0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,  0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f,  0.0f, 0.0f));

		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f,   0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, -0.5f,  1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,   0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f,   0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.2f, -0.5f, 0.0f, 0.0f));
        vertices.push_back(Vertex(0.5f, -0.2f, -0.5f,  1.0f, 0.0f));
        vertices.push_back(Vertex(0.5f, -0.2f, -0.6f,  1.0f, 1.0f));
        vertices.push_back(Vertex(0.5f, -0.2f, -0.6f,  1.0f, 1.0f));
        vertices.push_back(Vertex(-0.5f, -0.2f, -0.6f, 0.0f, 1.0f));
        vertices.push_back(Vertex(-0.5f, -0.2f, -0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.6f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.6f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, 0.6f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, -0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(-0.6f, -0.2f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.6f, -0.2f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.6f, -0.2f, 0.5f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.2f, 0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, -0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.6f, -0.2f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.6f, -0.2f, -0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.6f, -0.2f, 0.5f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.2f, 0.5f, 0.0f, 0.0f));
	}

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/preg.png", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 3);
		glBindVertexArray(m_vao);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, p);
		model = glm::scale(model, glm::vec3(25, 10, 25));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
};
class ceiling : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
public:
	ceiling(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		vertices.push_back(Vertex(-4.0f, 0.5f, -4.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(4.0f, 0.5f, -4.0f, 4.0f, 0.0f));
		vertices.push_back(Vertex(4.0f, 0.5f, 4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(4.0f, 0.5f, 4.0f, 4.0f, 4.0f));
		vertices.push_back(Vertex(-4.0f, 0.5f, 4.0f, 0.0f, 4.0f));
		vertices.push_back(Vertex(-4.0f, 0.5f, -4.0f, 0.0f, 0.0f));
	}

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/n.png", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
		glBindVertexArray(m_vao);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, p);
		model = glm::scale(model, glm::vec3(25, 10, 25));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
};
class laminate : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
public:
	laminate(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		vertices.push_back(Vertex(-4.0f, -0.5f, -4.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(4.0f, -0.5f, -4.0f, 50.0f, 0.0f));
		vertices.push_back(Vertex(4.0f, -0.5f, 4.0f, 50.0f, 50.0f));
		vertices.push_back(Vertex(4.0f, -0.5f, 4.0f, 50.0f, 50.0f));
		vertices.push_back(Vertex(-4.0f, -0.5f, 4.0f, 0.0f, 50.0f));
		vertices.push_back(Vertex(-4.0f, -0.5f, -4.0f, 0.0f, 0.0f));
	}

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/p.jpg", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 1);
		glBindVertexArray(m_vao);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, p);
		model = glm::scale(model, glm::vec3(25, 10, 25));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
};
class frontM : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
	float m_speed = 10;
	bool live = true;
public:
	frontM(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.f, 0.0f, 0.0f));
	}
	void Animate(glm::vec3 dd, float timeDelta)
	{
		p += timeDelta * m_speed * dd;
	}

	glm::vec3& Position() { return p; }
	void SetPosition(glm::vec3& ps) { p = ps; };
	bool checkIfHit() const { return live; }
	void getHit() { live = false; }

	float XMax() const { return p.x + 1.5f; }
	float XMin() const { return p.x - 1.5f; }
	float YMax() const { return p.y + 1.5f; }
	float YMin() const { return p.y - 1.5f; }
	float ZMax() const { return p.z + 0; }
	float ZMin() const { return p.z - 0; }

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/block.png", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	virtual void Draw(ShaderProgram & shader)
	{
		if (live == true)
		{
			glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 5);
			glBindVertexArray(m_vao);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p);
			model = glm::scale(model, glm::vec3(3, 3, 3));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);
		}
	}

};
class leftM : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
	float m_speed = 10;
	bool live = true;
public:
	leftM(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{
		vertices.push_back(Vertex(0.f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.f, -0.5f, 0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.f, 0.5f, 0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.f, 0.5f, 0.5f, 1.0f, 1.0f));
		vertices.push_back(Vertex(0.f, 0.5f, -0.5f, 0.0f, 1.0f));
		vertices.push_back(Vertex(0.f, -0.5f, -0.5f, 0.0f, 0.0f));
	}
	void Animate(glm::vec3 dd, float timeDelta)
	{
		p += timeDelta * m_speed * dd;
	}
	glm::vec3& Position() { return p; }
	void SetPosition(glm::vec3& ps) { p = ps; };
	bool checkIfHit() const { return live; }
	void getHit() { live = false; }

	float XMax() const { return p.x + 0; }
	float XMin() const { return p.x - 0; }
	float YMax() const { return p.y + 1.5f; }
	float YMin() const { return p.y - 1.5f; }
	float ZMax() const { return p.z + 1.5f; }
	float ZMin() const { return p.z - 1.5f; }

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/block.png", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		if (live == true)
		{
			glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 5);
			glBindVertexArray(m_vao);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p);
			model = glm::scale(model, glm::vec3(3, 3, 3));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);
		}
	}
};
class shot : public Drawable
{
private:
	glm::vec3 p;
	std::vector<Vertex> vertices;
	bool ready = true;
	float speed = 70;
	glm::vec3 newd;
public:
	shot(const glm::vec3& pos) :
		Drawable(),
		p(pos)
	{

		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,  0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,  1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,  0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,    0.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,   1.0f, 0.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,   0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f,    0.0f, 0.0f));

		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,    0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f,   1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,   0.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,    0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, -0.5f, 0.5f,   1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f,  0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f));

		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,0.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(0.5f, 0.5f, 0.5f,  1.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f));
		vertices.push_back(Vertex(-0.5f, 0.5f, -0.5f,0.0f, 0.0f));
	}
	glm::vec3& Position() { return p; }
	void SetPosition(glm::vec3& ps) { p = ps; };
	bool Ready() { return ready; }
	void setReady(bool s) { ready = s; }

	float XMax() const { return p.x + 0.05f; }
	float XMin() const { return p.x - 0.05f; }
	float YMax() const { return p.y + 0.05f; }
	float YMin() const { return p.y - 0.05f; }
	float ZMax() const { return p.z + 0.05f; }
	float ZMin() const { return p.z - 0.05f; }


	void Animate(glm::vec3 d, float timeDelta)
	{
		if (ready == true)
		{
			p = cameraPos;

			newd = d;
		}
		else
		{
			p += timeDelta * speed * newd;
		}

	}

	virtual void CreateVAO()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, s));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		{
			int width, height, nrChannels;
			std::shared_ptr<unsigned char> pData = std::shared_ptr<unsigned char>(stbi_load("Resources/b.jpg", &width, &height, &nrChannels, 0), stbi_image_free);
			if (!pData)
				throw std::exception("Failed to load texture");

			unsigned texture;
			glGenTextures(1, &texture);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData.get());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	virtual void Draw(ShaderProgram& shader)
	{
		if (ready == false)
		{
			glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 6);
			glBindVertexArray(m_vao);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p);
			model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);
		}
	}
};
class Game
{
public:
	std::unique_ptr<ShaderProgram> shader;
	vector<bool> moveXY;
	vector<unique_ptr<frontM>> m1;
	vector<unique_ptr<leftM>> m2;

	Map m = Map(glm::vec3(0.0f, 0.0f, 0.0f));
	ceiling c = ceiling(glm::vec3(0.0f, 0.0f, 0.0f));
	laminate l = laminate(glm::vec3(0.0f, 0.0f, 0.0f));
	crates cr = crates(glm::vec3(0.0f, 0.0f, 0.0f));
	shot s = shot(glm::vec3(0.0f, 0.0f, 0.0f));

	int score = 0;
	int ammo = 20;
	
	void LoadfrontM() 
	{
		m1.push_back(make_unique<frontM>(glm::vec3(4.0f, -2.0f, 22.5f)));
		m1.push_back(make_unique<frontM>(glm::vec3(-2.0f, 2.0f, 19.5f)));
		moveXY.push_back(true);
		moveXY.push_back(false);
		m1.push_back(make_unique<frontM>(glm::vec3(4.0f, -2.0f, -90.5f)));
		m1.push_back(make_unique<frontM>(glm::vec3(-2.0f, 2.0f, -80.5f)));
		m1.push_back(make_unique<frontM>(glm::vec3(5.0f, 3.0f,  -50.0f)));
		m1.push_back(make_unique<frontM>(glm::vec3(-4.0f,-3.0f, -65.0f)));
	}
	void LoadleftM()
	{
		m2.push_back(make_unique<leftM>(glm::vec3(22.5f, -2.0f,  4.0f )));
		m2.push_back(make_unique<leftM>(glm::vec3(19.5f, 2.0f,   -2.0f)));
		moveXY.push_back(true);
		moveXY.push_back(false);
		m2.push_back(make_unique<leftM>(glm::vec3(-30.5f, -2.0f, 4.0f )));
		m2.push_back(make_unique<leftM>(glm::vec3(-30.5f, 2.0f,  -2.0f)));
		m2.push_back(make_unique<leftM>(glm::vec3(-30.0f, 3.0f,  5.0f )));
		m2.push_back(make_unique<leftM>(glm::vec3(-35.0f, -3.0f, -4.0f)));
	}
	void CreateVAOs()
	{
		LoadfrontM();
		LoadleftM();
		shader.reset(new ShaderProgram("Shaders/mvp.vert", "Shaders/fragment.frag"));
		shader->use();

		m.CreateVAO();
		c.CreateVAO();
		l.CreateVAO();
		cr.CreateVAO();
		s.CreateVAO();

		for (int i = 0; i < m1.size(); i++) {
			m1.at(i).get()->CreateVAO();
		}
		for (int i = 0; i < m2.size(); i++) {
			m2.at(i).get()->CreateVAO();
		}
	};
	void Animate(GLFWwindow* Window, float timeDelta)
	{
		for (int i = 0; i < m1.size(); i++)
		{
			if (m1.at(i).get()->checkIfHit() == true)
			{
				if (m1.at(i).get()->Position().x <= 10.5 && moveXY.at(0) == false &&i == 0 )
				{
					m1.at(i).get()->Animate(glm::vec3(1.f, 0.f, 0.f), timeDelta);
					if (m1.at(i).get()->Position().x > 10.5)
					{
						moveXY.at(0) = true;
					}
				}
				else if (m1.at(i).get()->Position().x >= -10.5 && moveXY.at(0) == true && i == 0 )
				{
					m1.at(i).get()->Animate(glm::vec3(-1.f, 0.f, 0.f), timeDelta);
					if (m1.at(i).get()->Position().x < -10.5)
					{
						moveXY.at(0) = false;
					}
				}
				if (m1.at(i).get()->Position().x <= 10.5 && moveXY.at(1) == false && i == 1)
				{
					m1.at(i).get()->Animate(glm::vec3(1.f, 0.f, 0.f), timeDelta);
					if (m1.at(i).get()->Position().x > 10.5)
					{
						moveXY.at(1) = true;
					}
				}
				else if (m1.at(i).get()->Position().x >= -10.5 && moveXY.at(1) == true && i == 1)
				{
					m1.at(i).get()->Animate(glm::vec3(-1.f, 0.f, 0.f), timeDelta);
					if (m1.at(i).get()->Position().x < -10.5)
					{
						moveXY.at(1) = false;
					}
				}
			}
		}
		for (int i = 0; i < m2.size(); i++)
		{
			if (m2.at(i).get()->checkIfHit() == true)
			{
				if (m2.at(i).get()->Position().y <= 3 && moveXY.at(2) == false && i == 0)
				{
					m2.at(i).get()->Animate(glm::vec3(0.f, 1.f, 0.f), timeDelta);
					if (m2.at(i).get()->Position().y > 3)
					{
						moveXY.at(2) = true;
					}
				}
				else if (m2.at(i).get()->Position().y >= -3 && moveXY.at(2) == true && i == 0)
				{
					m2.at(i).get()->Animate(glm::vec3(0.f, -1.f, 0.f), timeDelta);
					if (m2.at(i).get()->Position().y < -3)
					{
						moveXY.at(2) = false;
					}
				}
				if (m2.at(i).get()->Position().y <= 3 && moveXY.at(3) == false && i == 1)
				{
					m2.at(i).get()->Animate(glm::vec3(0.f, 1.f, 0.f), timeDelta);
					if (m2.at(i).get()->Position().y > 3)
					{
						moveXY.at(3) = true;
					}
				}
				else if (m2.at(i).get()->Position().y >= -3 && moveXY.at(3) == true && i == 1)
				{
					m2.at(i).get()->Animate(glm::vec3(0.f, -1.f, 0.f), timeDelta);
					if (m2.at(i).get()->Position().y < -3)
					{
						moveXY.at(3) = false;
					}
				}
			}
		}
		if (s.Ready() == true && readyShotint == 1 && ammo>0 )
		{
			s.setReady(false);			
		}
		s.Animate(cameraFront, timeDelta);
		if (s.XMax() >= m.XMax() || s.XMin() <= m.XMin()|| s.YMax() >= m.YMax() || s.YMin() <= m.YMin() || s.ZMax() >= m.ZMax() || s.ZMin() <= m.ZMin() )
		{
			s.setReady(true);
			ammo--;
			cout << "Ammo left:" << ammo << endl;
		}

		for (int i = 0; i < m1.size(); i++) {
			if (m1.at(i).get()->checkIfHit() == true)
			{
				if (s.XMax() <= m1.at(i).get()->XMax() && s.XMin() >= m1.at(i).get()->XMin() && s.YMax() <= m1.at(i).get()->YMax() && s.YMin() >= m1.at(i).get()->YMin() && s.ZMax() >= m1.at(i).get()->ZMin()-1 && s.ZMin()<=m1.at(i).get()->ZMax()+1)
				{
					s.setReady(true);
					m1.at(i).get()->getHit();
					score++;
					ammo--;
					cout << "Score:" << score << endl;
					cout << "Ammo left:" << ammo << endl;
				}
			}
		}

		for (int i = 0; i < m2.size(); i++) {
			if (m2.at(i).get()->checkIfHit() == true)
			{
				if (s.XMax() >= m2.at(i).get()->XMin() && s.YMax() <= m2.at(i).get()->YMax() && s.YMin() >= m2.at(i).get()->YMin() && s.ZMax() <= m2.at(i).get()->ZMax() && s.ZMin() >= m2.at(i).get()->ZMin())
				{
					s.setReady(true);
					m2.at(i).get()->getHit();
					score++;
					ammo--;
					cout << "Score:" << score << endl;
					cout << "Ammo left:" << ammo << endl;
				}
			}
		}
		AnimateCamera(Window, timeDelta);
		
		if (score == 12)
		{
			cout << "WIN" << endl;
		}
		if (ammo == 0&& score!=12)
		{
			cout << "Defeat" << endl;
		}
	};
	void AnimateCamera(GLFWwindow* Window, float timeDelta)
	{
		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		SetProjection(projection);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		SetView(view);
	};

	void Draw()
	{
		m.Draw(*shader);
		c.Draw(*shader);
		l.Draw(*shader);
		cr.Draw(*shader);
		s.Draw(*shader);
		for (int i = 0; i < m1.size(); i++) {
			if (m1.at(i).get()->checkIfHit() == true)
			{
				m1.at(i).get()->Draw(*shader);
			}
		}
		for (int i = 0; i < m2.size(); i++) {
			if (m2.at(i).get()->checkIfHit() == true)
			{
				m2.at(i).get()->Draw(*shader);
			}
		}

	};

	void SetView(const glm::mat4& view) { glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view)); }
	void SetProjection(const glm::mat4& proj) { glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj)); }

	~Game()
	{
	};
};
void on_error(int code, const char* text)
{
	std::cout << "Error code: " << code << " Error text: " << text << std::endl;
};


int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	glfwInit();


	try
	{
		stbi_set_flip_vertically_on_load(true);

		glfwSetErrorCallback(on_error);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
		glfwMakeContextCurrent(window);
		if (window == NULL)
			throw std::exception("Failed to create GLFW window");


		glfwMakeContextCurrent(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::exception("Failed to initialize OpenGL context");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);

		Game Game;
		glfwSetWindowUserPointer(window, (void*)&Game);

		Game.CreateVAOs();
		double newdt = glfwGetTime();
		double olddt = glfwGetTime();
		double frameStart = glfwGetTime();
		double lastFrameStart = glfwGetTime();
		bool str = true;
		while (!glfwWindowShouldClose(window))
		{
			newdt = glfwGetTime();

			frameStart = glfwGetTime();
			float timeDelta = (float)(frameStart - lastFrameStart);
			deltaTime = (float)(frameStart - lastFrameStart);

			glfwPollEvents();
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Game.Animate(window, timeDelta);
			Game.Draw();

			glfwSwapBuffers(window);
			lastFrameStart = frameStart;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Unexpected error: " << e.what() << std::endl;
		std::cin.get();
	}
	glfwTerminate();
	return 0;
}