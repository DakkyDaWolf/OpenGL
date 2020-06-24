#version 440 core

layout (location = 0) in vec3 vPos;

uniform mat4 WorldViewProjection;

void main()
{
    gl_Position = WorldViewProjection * vec4(vPos, 1.0);
}