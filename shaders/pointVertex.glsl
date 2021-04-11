#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 VertexColor;

out vec4 Color;

void main()
{
    gl_PointSize = 5.0;
    Color = VertexColor;
    gl_Position = vec4(VertexPosition, 1.0);
}
