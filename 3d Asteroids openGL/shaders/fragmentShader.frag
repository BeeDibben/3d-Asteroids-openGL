#version 460
//Colour value to send to next stage
out vec4 FragColor;
//in vec2 texCoordFrag;
//uniform sampler2D texturein;

in vec3 outColour;


void main()
{
    //FragColor = texture(texturein, texCoordFrag); //Texturing

    fragColor = vec4(outColor, 1.0f);
}