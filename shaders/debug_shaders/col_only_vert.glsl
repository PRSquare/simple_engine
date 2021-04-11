#version 430

layout (location = 0) in vec3 VertexPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 modelMatrix;
uniform mat4 rotationMatrix;

out vec4 Color;

void main()
{
    Color = vec4(1.0, 0.0, 0.0, 0.3);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(VertexPosition, 1.0);
}
