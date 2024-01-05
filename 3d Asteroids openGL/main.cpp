#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "main.h"
#include "shaders/LoadShaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

GLuint program;

// Time controls
double previousFrameTime = 0;
double currentFrameTime;
double deltaTime;

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

    //Load shaders
    ShaderInfo shaders[] =
    {
        { GL_VERTEX_SHADER, "shaders/vertexShader.vert" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    program = LoadShaders(shaders);
    glUseProgram(program);

    glViewport(0, 0, 1280, 720);//Defines Window size

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //Sets framebuffer_size_callback as the function called for the window resizing event

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0,1,3, // tri 1
        1,2,3 // tri 2
    };

    //Sets index of VAO
    glGenVertexArrays(NumVAOs, VAOs);
    //Binds VAO to a buffer
    glBindVertexArray(VAOs[0]);
    //Sets indexes of all required buffer objects
    glGenBuffers(NumBuffers, Buffers);

    //Binds VAO to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Allocates buffer memory for the vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // binds VAO buffer for Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //Allocates vertex attribute memory for vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Unbinding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Texturing here
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int width, height, colourChanels;
    unsigned char* data = stbi_load("media/wall.jpg", &width, &height, &colourChanels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Faild to load texture";
        return -1;
    }
    stbi_image_free(data);


    while (glfwWindowShouldClose(window) == false) {
        frameTimeUpdate();
        //input
        ProcessUserInput(window);

        //Rendering
        glClearColor(0.25f, 0.0f, 1.0f, 1.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT); //Clears the colour buffer

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAOs[0]); //Bind buffer object to render
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Render buffer object

        //Refreshing
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

void frameTimeUpdate() {
    // Used to track time since last frame, in order to keep physics and movement consistent.
    // In the explainations, n = the current frame count
    previousFrameTime = currentFrameTime; // Overwites n-2, with n-1
    currentFrameTime = glfwGetTime(); // Overwrites n-1 with n
    deltaTime = currentFrameTime - previousFrameTime; // Subtracts n-1 from n to compare the time.
    
}