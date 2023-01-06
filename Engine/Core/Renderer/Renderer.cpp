﻿#include "Renderer.h"
#include <Windows.h>
#include <glad/glad.h>
#include "ResourceManager.h"
#include "Core/GameObject.h"
#include "Core/Components/Transform.h"
#include "glm/gtx/transform.hpp"
#include "Library/glfw3.h"
#include "Util/Logger.h"
#include "Util/Events/EngineEvents.h"
#include "Util/Events/Events.h"

void error_callback(const int error, const char *msg)
{
	std::string s;
	s = " [" + std::to_string(error) + "] " + msg + '\n';
	LOG_ERROR(s);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::SetWindowTitle(const std::string& title) const
{
	if (window == nullptr)
	{
		LOG_ERROR("Failed to set window title");
		return;
	}

	glfwSetWindowTitle(window, title.c_str());
}

void Renderer::SetWindowSize(const glm::ivec2 value)
{
	if (window == nullptr)
	{
		LOG_ERROR("Failed to set window size");
		return;
	}
	
	windowSize = value;
	
	const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(value.x), 
											static_cast<float>(value.y), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	
	glfwSetWindowSize(window, value.x, value.y);
}

bool Renderer::createWindow(const std::string &windowName)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		LOG_ERROR("Failed to create init GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(1, 1, windowName.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		LOG_ERROR("Failed to create GLFW window");
		return false;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return false;
	}
	
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	return true;
}

void Renderer::cleanup() const
{
	if (window != nullptr)
	{
		glfwDestroyWindow(window);
	}
}

void Renderer::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (SpriteRenderer* spriteRenderer : renderQueue)
	{
		const Transform* transform = dynamic_cast<Transform*>(spriteRenderer->owner->hasComponentInternal(typeid(Transform)));
		
		spriteRenderer->shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(transform->GetPosition(), 0.0f));  

		model = translate(model, glm::vec3(0.5f * transform->GetScale().x, 0.5f * transform->GetScale().y, 0.0f)); 
		model = rotate(model, glm::radians(transform->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = translate(model, glm::vec3(-0.5f * transform->GetScale().x, -0.5f * transform->GetScale().y, 0.0f));

		model = scale(model, glm::vec3(transform->GetScale(), 1.0f)); 
  
		spriteRenderer->shader.SetMatrix4("model", model);
		spriteRenderer->shader.SetVector3f("spriteColor", spriteRenderer->color);
  
		glActiveTexture(GL_TEXTURE0);
		spriteRenderer->texture.Bind();

		glBindVertexArray(spriteRenderer->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}

void Renderer::initialize()
{
	Disunity::Events::Instance()->subscribe(this, &Renderer::addToRenderQueue);
	Disunity::Events::Instance()->subscribe(this, &Renderer::removeFromRenderQueue);
}

void Renderer::addToRenderQueue(OnSpriteRendererComponentStarted* event)
{
	event->spriteRenderer->shader = ResourceManager::GetShader("sprite");
	renderQueue.push_back(event->spriteRenderer);
}

void Renderer::removeFromRenderQueue(OnSpriteRendererComponentRemoved* event)
{
	renderQueue.erase(std::ranges::remove(renderQueue, event->spriteRenderer).begin(), renderQueue.end());
}
