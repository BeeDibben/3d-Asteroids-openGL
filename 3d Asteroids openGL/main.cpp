#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>

using namespace std;

//STD
#include <iostream>

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

using namespace std;

int main()
{
    //Initialisation of GLFW
    glfwInit();
    //Initialisation of 'GLFWwindow' object
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Lab5", NULL, NULL);

    //Checks if window has been successfully instantiated
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    //Binds OpenGL to window
    glfwMakeContextCurrent(window);

    //Initialises GLEW
    glewInit();

    //Defines Window size
    glViewport(0, 0, 1280, 720);

    


    return 0; //Ends the current runtime
}