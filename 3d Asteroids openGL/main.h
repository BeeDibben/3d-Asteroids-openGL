#pragma once

#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //Window Resize function

void ProcessUserInput(GLFWwindow* windowIn); //Captures user input for a given window

void frameTimeUpdate(); // records change in time since the last frame

float getDeltaTime(); // Returns the change in time since the last frame

void favouriteColour(); 

void mouseCallback(GLFWwindow* window, double xpos, double ypos); // Returns current value of the mouse

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); // receives button input data