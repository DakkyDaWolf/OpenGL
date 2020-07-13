#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

uniform mat4 ScreenspaceProjection;

out vec2 TexCoords;

void main()
{
    TexCoords = TextureCoordinate;
    gl_Position = ScreenspaceProjection * Position;
}