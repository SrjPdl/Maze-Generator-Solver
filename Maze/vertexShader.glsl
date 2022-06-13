#version 440 core

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;

out vec4 color;

//uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
    gl_Position =  projection * view  * vec4(iPosition,1.f);

    color = iColor;
}