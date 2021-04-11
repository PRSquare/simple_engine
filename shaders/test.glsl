#version 430
layout(triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main()
{
    gl_Position = vec4(0, 0, 0, 1);
    EmitVertex();
    gl_Position = vec4(1, 0, 0, 1);
    EmitVertex();
    gl_Position = vec4(0, 1, 0, 1);
    EmitVertex();
    EndPrimitive();
}
