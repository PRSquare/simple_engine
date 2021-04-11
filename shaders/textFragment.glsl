#version 430

in vec2 TexCord;
//in vec3 Color;
out vec4 FragColor;
uniform sampler2D Tex1;// = 0;
uniform vec4 textColor;

void main()
{
    vec4 TexColor = texture2D( Tex1, TexCord );
    if(TexColor.a > 0.2)
        TexColor = textColor;
    FragColor = TexColor;
}
