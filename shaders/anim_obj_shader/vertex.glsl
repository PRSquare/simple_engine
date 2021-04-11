#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCord;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 WIdAndWeight[];

uniform float wCount;
uniform mat4 bones[64];

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 modelMatrix;
uniform mat4 rotationMatrix;

uniform vec3 lightingVec;

layout(location = 0) out vec2 TexCord;
layout(location = 1) out float lightIntencity;

void main()
{
    vec4 VP =  modelMatrix * vec4(VertexPosition, 1.0);
    vec4 VN = rotationMatrix * vec4(VertexNormal, 1.0);
    float Ld = 0.4;
    vec3 lv = -lightingVec;
    vec3 tNorm =  VN.xyz ;
    vec3 FragPos = vec3(modelMatrix * vec4(VP.xyz, 1.0));
    vec3 s = normalize(lv) ;
    lightIntencity = Ld * max( dot(s, tNorm), 0.0 );

    gl_PointSize = 5.0;

    //for(int i = 0; i < WCount; ++i)
    //{
    //    VP = VP*bones[ WIdAndWeight[0] ] * WIdAndWeight[1];
    //}

    TexCord = VertexTexCord;
    vec4 POS = projectionMatrix * viewMatrix* VP;
    gl_Position = POS;
}
