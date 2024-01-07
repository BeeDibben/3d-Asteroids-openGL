#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "main.h"
#include "shaders/LoadShaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang/irrKlang.h>


#define RENDER_DISTANCE 250
#define MAP_SIZE RENDER_DISTANCE*RENDER_DISTANCE



// - - - OpenGL processes - - - //


using namespace std;
using namespace glm;
using namespace irrklang;

//VAO vertex attribute positions in correspondence to vertex attribute type
enum VAO_IDs { Triangles, Indices, Colours, Textures, NumVAOs = 2 };
//VAOs
GLuint VAOs[NumVAOs];

//Buffer types
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
//Buffer objects
GLuint Buffers[NumBuffers];

GLuint program;
 
mat4 transform;
mat4 model;
mat4 view;
mat4 projection;

// Audio setup
ISoundEngine* soundEngine = createIrrKlangDevice();



// - - -Game data - - - //

// Window Data

int windowWidth = 1200;
int windowHeight = 720;

    // Time controls //
float previousFrameTime = 0;
float currentFrameTime;
float deltaTime;

//Object data
float rotation = 10;

//Camera
bool firstMouse = true;

vec3 camPosition = vec3(0.0f, 0.0f, 3.0);
vec3 camFront = vec3(0.0f, 0.0f, -1.0f);
vec3 camUp = vec3(0.0f, 1.0f, 0.0f);

float camPitch = 0.0f;
float camYaw = -90.0f;

float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;

float FOV = 50.0f;

float forwardSpeed = 8;
float directionalSpeed = 5;

float sensitivity = 0.005f * deltaTime;

//PCG (procedural content generation)


int main(int argc, char *argv[])
{
    glfwInit();//Inirialises glfw

    //loades media files into memory to prevent lag
    soundEngine->play2D("media/laser_sfx.wav", false, true);
    soundEngine->play2D("media/Space_Game_Ambient.wav", false, true);
    soundEngine->setSoundVolume(0.5);


    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Asteroids 3D", NULL, NULL); //Initialisation of 'GLFWwindow' object

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

    // glfw setup

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //Sets framebuffer_size_callback as the function called for the window resizing event

    glEnable(GL_DEPTH_TEST); // Ensures accurate depth calculation for rendering

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // removes cursor from veiw when engine is running
    
    glfwSetCursorPosCallback(window, mouseCallback); // Sets the function for retreiveing mouse data

    glfwSetMouseButtonCallback(window, mouse_button_callback); //sets the funciton that will handle mouse button inputs

    //Models info

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cubePositions[] = {
    vec3(0.0f,  0.0f,  0.0f),
    vec3(2.0f,  5.0f, -15.0f),
    vec3(-1.5f, -2.2f, -2.5f),
    vec3(-3.8f, -2.0f, -12.3f),
    vec3(2.4f, -0.4f, -3.5f),
    vec3(-1.7f,  3.0f, -7.5f),
    vec3(1.3f, -2.0f, -2.5f),
    vec3(1.5f,  2.0f, -2.5f),
    vec3(1.5f,  0.2f, -1.5f),
    vec3(-1.3f,  1.0f, -1.5f)
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

    //Allocates vertex attribute memory for vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //Index of vertex attribute for vertex shader
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    unsigned char* data = stbi_load("media/drawnWall.jpg", &width, &height, &colourChanels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Faild to load texture";
        return -1;
    }
    stbi_image_free(data);


    // Model View Projection (MVP)
    GLint modelIndex = glGetUniformLocation(program, "model");
    GLint viewIndex = glGetUniformLocation(program, "view");
    GLint projectionIndex = glGetUniformLocation(program, "projection");

    

  // - - - Game Loop - - - //  

    while (glfwWindowShouldClose(window) == false) {
        frameTimeUpdate();
        //input
        ProcessUserInput(window);



        
        
        rotation = rotation + (50 * deltaTime);

        view = mat4(1.0f);
        projection = mat4(1.0f);



        projection = perspective(radians(45.0f), (float)(windowWidth / windowHeight), 0.1f, 100.0f);
        view = lookAt(camPosition, camPosition + camFront, camUp); // Camera



        //refreshing the mvp
        glUniformMatrix4fv(viewIndex, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, value_ptr(projection));



          
        //Rendering
        glClearColor(0.25f, 0.0f, 1.0f, 1.0f); //Colour to display on cleared window
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Clears the colour buffer

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAOs[0]); //Bind buffer object to render
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Render buffer object
        
        for (int i = 0; i < 10; i++)
        {
            float angle = 20.0f * i * deltaTime;

            
            model = mat4(1.0f);
            //model = scale(model, vec3(1.0f, 1.0f, 1.0f));
            model = rotate(model, float(radians(rotation + angle)), cubePositions[i]);
            model = translate(model, cubePositions[i]);
            


            glUniformMatrix4fv(modelIndex, 1, GL_FALSE, value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        //Refreshing
        glfwSwapBuffers(window); //Swaps the colour buffer
        glfwPollEvents(); //Queries all glfw events

        

        

    }
    
    glfwTerminate();

    return 0; //Ends the current runtime
}








// - - - Function Defintions - - - //



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
        camPosition += (forwardSpeed*deltaTime) * camFront;
    }
    //Move Backward
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS) {
        cout << "S pressed\n";
        camPosition -= (directionalSpeed*deltaTime) * camFront;
    }
    //Move Left
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS) {
        cout << "A pressed\n";
        camPosition -= (normalize(cross(camFront, camUp))*deltaTime) * directionalSpeed;
    }
    //Move Right
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS) {
        cout << "A pressed\n";
        camPosition += (normalize(cross(camFront, camUp))*deltaTime) * directionalSpeed;
    }
    //Move Up
    if (glfwGetKey(WindowIn, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cout << "Space pressed\n"; 
        camPosition +=  camUp * (directionalSpeed*deltaTime);
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cout << "L-Shift pressed\n"; 
        camPosition -=  camUp * (directionalSpeed*deltaTime);
    }
    

    //bonus controll
    if (glfwGetKey(WindowIn, GLFW_KEY_M) == GLFW_PRESS) {
        favouriteColour();
    }
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camYaw += xoffset;
    camPitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (camPitch > 89.0f)
        camPitch = 89.0f;
    if (camPitch < -89.0f)
        camPitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
    front.y = sin(glm::radians(camPitch));
    front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
    camFront = glm::normalize(front);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // plays an audio sample using the library IRRKLANG, as instructed on the LEARNOPENGL website: https://learnopengl.com/In-Practice/2D-Game/Audio
        cout << "Left Mouse button pressed.\n";
        soundEngine->play2D("media/laser_sfx.wav", false); // Royalty free sfx
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        cout << "Right Mouse button pressed.\n";
        if (soundEngine->isCurrentlyPlaying("media/Space_Game_Ambient.wav") == true) {
            cout << "space music is playing\n";
            soundEngine->play2D("media/Space_Game_Ambient.wav", true, false);
        }
    }

}

void frameTimeUpdate() {
    // Used to track time since last frame, in order to keep physics and movement consistent.
    // In the explainations, n = the current frame count
    previousFrameTime = currentFrameTime; // Overwites n-2, with n-1
    currentFrameTime = glfwGetTime(); // Overwrites n-1 with n
    deltaTime = currentFrameTime - previousFrameTime; // Subtracts n-1 from n to compare the time.
}


void favouriteColour() {
    cout << "#228b22\n";
}