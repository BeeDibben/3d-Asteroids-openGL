#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "main.h"

using namespace std;


using namespace std;

int main(int argc, char *argv[])
{
    glfwInit();//Inirialises glfw

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Asteroids 3D", NULL, NULL); //Initialisation of 'GLFWwindow' object

    if (window == NULL) //Checks if window has been successfully instantiated
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //Binds OpenGL to window

    glewInit();//Initialises GLEW

    glViewport(0, 0, 1280, 720);//Defines Window size

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //Sets framebuffer_size_callback as the function called for the window resizing event


    while (glfwWindowShouldClose(window) == false) {
        //input
        ProcessUserInput(window);

        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all glfw events


    }
    
    glfwTerminate();

    return 0; //Ends the current runtime
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //Resizes the window based on desired size
    glViewport(0, 0, width, height);
}

void ProcessUserInput(GLFWwindow* WindowIn) {
    //Close Current Window
    if (glfwGetKey(WindowIn, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(WindowIn, true);
    }


    // -- Movement controlls -- //
    //Move forward
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS) {
        cout << "W pressed\n";
    }
    //Move Backward
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS) {
        cout << "S pressed\n";
    }
    //Move Left
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS) {
        cout << "A pressed\n";
    }
    //Move Right
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS) {
        cout << "D pressed\n";
    }
    //Move Up
    if (glfwGetKey(WindowIn, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cout << "Space pressed\n"; 
        
    }
}