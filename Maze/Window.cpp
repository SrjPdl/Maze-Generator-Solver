#include "Window.h"

Window::Window()
	:windowWidth(800),
	windowHeight(600),
	windowPosition(glm::vec2(0, 0)),
	window(nullptr)
{

}

int32_t Window::frameBufferWidth = 0;
int32_t Window::frameBufferHeight = 0;

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Window::setWindowHints()
{
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
}

void Window::handleResize(GLFWwindow* window, int width, int height)
{
	frameBufferWidth = width;
	frameBufferHeight = height;
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

void Window::create(const char* title,const uint32_t windowWidth, const uint32_t windowHeight, const glm::vec2 windowPosition)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowPosition = windowPosition;
	
	//Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "ERROR::FAILED_TO_INITIALIZE_GLFW" << std::endl;
	}

	//Create a windowed mode window and its OpenGL context
	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, title, NULL, NULL);

	//Check if window was created Sucessfully
	if (!this->window)
	{
		std::cout << "ERROR::FAILED_TO_CREATE_WINDOW" << std::endl;
		glfwTerminate();
	}

	//Set Window Position
	glfwSetWindowPos(this->window, this->windowPosition.x, this->windowPosition.y);

	//Make the window's context current
	glfwMakeContextCurrent(this->window);

	//get and set frameBufferWidth and frameBufferHeight
	glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);


	glfwSetWindowSizeCallback(this->window,handleResize);
}

GLFWwindow* Window::getWindow() const
{
	return this->window;
}

glm::vec2 Window::getViewportSize() const
{
	return (glm::vec2(frameBufferWidth, frameBufferHeight));
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(this->window);
}
