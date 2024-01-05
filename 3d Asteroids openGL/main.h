#pragma once

#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //Window Resize function

void ProcessUserInput(GLFWwindow* windowIn); //Captures user input for a given window

void frameTimeUpdate(); // records change in time since the last frame