#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCord;
layout (location = 2) in vec3 VertexNormal;

uniform vec2 tex_cord_offset;
uniform vec3 position_offset;
uniform float scale;

out vec2 TexCord;
//out vec3 Color;

void main()
{
    TexCord = VertexTexCord + tex_cord_offset;
    gl_Position = vec4(VertexPosition * scale + position_offset, 1.0);
}
