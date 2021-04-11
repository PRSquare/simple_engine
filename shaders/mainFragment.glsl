#version 430

layout(location = 0) in vec2 TexCord;
layout(location = 1) in float bcfv;

in vec4 VN;
in  vec3 eye_cords;
in float z;

float lightIntencity;
out vec4 FragColor;

uniform vec3 lightingVec;
uniform sampler2D Tex1;

vec4 Fogcolor = vec4(0.0, 0.0, 0.0, 1.0);
float fStart = 100;
float fEnd = 1;

float rand3D(in vec3 co){
   return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

bool is_ok(float a)
{
    return a/2.0 - int(a/2.0) == 0;
}


bool inRadius(vec2 pos1, vec2 pos2, float radius)
{
    float xl = pos2.x - pos1.x;
    float yl = pos2.y - pos1.y;
    return sqrt(xl*xl + yl*yl)/128 <= radius/128;
}

void main()
{
    if(!gl_FrontFacing)
        discard;

    vec4 TexColor;
    TexColor = texture2D( Tex1, TexCord );


    float Ld = 0.5;
    vec3 tNorm = VN.xyz ;
    vec3 s = normalize(-lightingVec);
    vec3 r = reflect(-s, tNorm);
    float norm_dot =  max(dot(s, tNorm), 0.0);
    lightIntencity = Ld * norm_dot;

    vec3 spec = vec3(0.0);
    if(norm_dot > 0.0)
        spec = vec3(0.5) * pow(max( dot(r, normalize(eye_cords.xyz)), 0.0), 2);


    vec3 lightColor= vec3(1.0, 1.0, 1.0);
    float ambStrength = 0.05;
    vec3 ambient = ambStrength * lightColor;

    vec4 result;
    // + vec3(TexColor) * lightIntencity + vec3(TexColor) * ambient
    //result = vec4( vec3(TexColor) * spec, TexColor.a);
    result = vec4(vec3(TexColor) * lightIntencity + vec3(TexColor) * ambient, TexColor.a);
//    if(true)
//    {
//        float col = rand3D(result.rgb);
//        result = mix(result, vec4(col, col, col, 1.0), 0.5);
//        //TexColor = vec4(col, col, col, 1.0);
//    }
    float fogFactor = (fEnd-z)/(fEnd-fStart);


    //FragColor = vec4(min(result.r, 1.0), min(result.g, 1.0), min(result.b, 1.0), min(result.a, 1.0));
    //FragColor = mix(result, Fogcolor, fogFactor);
    FragColor = result;
}
