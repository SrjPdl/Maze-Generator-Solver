#pragma once

#include "Shader.h"
#include "Window.h"
#include "Camera2D.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include<glm/ext/matrix_transform.hpp>

class Application
{
	private:
		const int WINDOW_WIDTH;
		const int WINDOW_HEIGHT;
		const glm::vec2 WINDOW_POSITION;
		Window window;
		Shader shader;
		glm::mat4 projection;

		int32_t graphWidthSlider;
		int32_t graphHeightSlider;

		int32_t graphWidth;
		int32_t graphHeight;
		int32_t oldGraphHeight;
		int32_t generatorSpeed; // controls the speed at which maze generated is rendered
		bool isGeneratePressed; 
		bool isSolvePressed;
		int32_t solverSpeed;

		int32_t startNodeX;
		int32_t startNodeY;

		int32_t endNodeX;
		int32_t endNodeY;

	private:
		void guiSetup();


	private:
		void setup();

		Application();

	public:
		Application(const Application& ) = delete;
		static Application& Get();
		void update();
		~Application();


};

