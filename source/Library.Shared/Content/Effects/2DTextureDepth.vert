#version 440 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureCoordinate;

uniform mat4 ScreenspaceProjection;

out VS_OUTPUT
{
	vec2 TextureCoords;
} OUT;

void main()
{
    gl_Position = ScreenspaceProjection * Position;
	OUT.TextureCoords = TextureCoordinate;
}