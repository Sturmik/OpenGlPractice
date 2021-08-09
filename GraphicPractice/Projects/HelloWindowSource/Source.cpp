#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Function for window size update
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Function for input processing
void processInput(GLFWwindow* window);
// Function for window color update
void windowColorUpdate(float& R, float& G, float& B, bool& stateColor, float incDecrRate = 0.01f);

int main()
{
	// Initialize GLFW
	glfwInit();
	// Set major and minor version of opengl 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set opengl profile type
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
	// For MAC OS X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Creates window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Set Context
	glfwMakeContextCurrent(window);
	// Sets function callback for window size updates
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Loads glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	float R, G, B;
	R = G = B = 0;
	bool stateColor = R;
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		//windowColorUpdate(R, G, B);
		// Screen clear
		glClearColor(R, G, B, 1.f);
		windowColorUpdate(R, G, B,stateColor, 0.01f);
		glClear(GL_COLOR_BUFFER_BIT);

		// input
		processInput(window);
		
		// rendering commands here
		
		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	// Clean/Delete all GLFW resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void windowColorUpdate(float& R, float& G, float& B, bool& stateColor, float incDecrRate)
{
	switch (stateColor)
	{
	case true:
		if (R < 1)
		{
			if (R <= 0)
			{
				std::cout << "Incrementing Red Color" << std::endl;
			}
			R += incDecrRate;
		}
		else
			if (G < 1)
			{
				if (G <= 0)
				{
					std::cout << "Incrementing Green Color" << std::endl;
				}
				G += incDecrRate;
			}
			else
				if (B < 1)
				{
					if (B <= 0)
					{
						std::cout << "Incrementing Blue Color" << std::endl;
					}
					B += incDecrRate;
				}
				else
				{
					stateColor = !stateColor;
				}
		break;
	case false:
		if (R > 0)
		{
			if (R > 1)
			{
				std::cout << "Decreasing Red Color" << std::endl;
			}
			R -= incDecrRate;
		}
		else
			if (G > 0)
			{
				if (G > 1)
				{
					std::cout << "Decreasing Green Color" << std::endl;
				}
				G -= incDecrRate;
			}
			else
				if (B > 0)
				{
					if (B > 1)
					{
						std::cout << "Decreasing Blue Color" << std::endl;
					}
					B -= incDecrRate;
				}
				else
				{
					stateColor = !stateColor;
				}
		break;
	}
}