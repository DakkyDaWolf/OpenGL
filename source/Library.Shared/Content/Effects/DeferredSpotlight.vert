#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

out VS_OUTPUT
{
	vec2 TexCoords;
} OUT;

void main()
{
    OUT.TexCoords = TextureCoordinate;
    gl_Position = Position;
}