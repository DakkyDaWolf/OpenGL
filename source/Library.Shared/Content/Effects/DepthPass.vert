#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;
layout (location = 2) in vec3 Normal;

uniform mat4 WorldViewProjection;

void main()
{
    gl_Position = WorldViewProjection * Position;
}