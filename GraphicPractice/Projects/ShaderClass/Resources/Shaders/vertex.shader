#version 330 core

layout(location = 0) in vec3 position;  // the position variable has attribute position 0
layout(location = 1) in vec3 color;  //the color variable has attribute position 1

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(position, 1.0); // directly give vec3 to vec4's constructor
    ourColor = color;
};