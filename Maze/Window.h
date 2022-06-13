#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Window
{
	private:
		uint32_t windowWidth;
		uint32_t windowHeight;
		static int32_t frameBufferWidth;
		static int32_t frameBufferHeight;
		glm::vec2 windowPosition;
		GLFWwindow* window;

	private:
		void setWindowHints();
		static void handleResize(GLFWwindow* window,int width,int height);

	public:
		Window();
		~Window();

		void create(const char* title,const uint32_t windowWidth, const uint32_t windowHeight, const glm::vec2 windowPosition);

		//getters
		GLFWwindow* getWindow() const;
		glm::vec2 getViewportSize() const;
		bool isOpen() const;

};

