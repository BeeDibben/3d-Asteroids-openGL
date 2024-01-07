#version 460
//Triangle position with values retrieved from main.cpp
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoordVert;
out vec2 texCoordFrag;

//mvp
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    //Triangle vertice sent through gl_Position to next stage
    gl_Position = projection*view*model*vec4(position.x, position.y, position.z, 1.0);
    texCoordFrag = texCoordVert;


   
}