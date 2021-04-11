#version 430

layout(location = 1) in float lightIntencity;

layout(location = 0) in vec2 TexCord;
out vec4 FragColor;
uniform sampler2D Tex1;

void main()
{
    if(!gl_FrontFacing)
        discard;


    vec3 lightColor= vec3(1.0, 1.0, 1.0);
    float ambStrength = 0.1;
    vec3 ambient = ambStrength * lightColor;

    vec4 TexColor = texture2D( Tex1, TexCord );
    //vec4 TexColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 result = vec4( vec3(TexColor) * lightIntencity + vec3(TexColor) * ambient, TexColor.a);

    FragColor = vec4(min(result.r, 1.0), min(result.g, 1.0), min(result.b, 1.0), min(result.a, 1.0));
}
