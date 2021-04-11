#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCord;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in float Weights[];

uniform float wCount;
uniform mat4 bones[64];

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 modelMatrix;
uniform mat4 rotationMatrix;

uniform vec3 cam_cords;

layout(location = 0) out vec2 TexCord;
layout(location = 1) out float bcfv;
out vec4 VN;
out vec3 eye_cords;
out float z;


void main()
{
    vec4 VP =  modelMatrix * vec4(VertexPosition, 1.0);
    VN = rotationMatrix * vec4(VertexNormal, 1.0);

    //eye_cords =  modelMatrix * viewMatrix * vec4(VertexPosition, 1.0);
    gl_PointSize = 5.0;

    bcfv = wCount;
    
    TexCord = VertexTexCord;
    vec4 POS = projectionMatrix * viewMatrix* VP;

    eye_cords = cam_cords;
    z = distance(cam_cords, VP.xyz);

    gl_Position = POS;
}
