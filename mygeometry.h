#pragma once
#include <GL/glew.h>
#include <fstream>

class myGeometry
{
public:
    GLuint vaoHandle;
    //myGeometry(float* , int); // verts only
    myGeometry(float* vert_array, int vert_array_size, float *uv_array = nullptr, int uv_array_size = 0 ,float *normal_array = nullptr, int normal_array_size = 0); // verts and normals
    myGeometry( const myGeometry & );
    myGeometry operator=(myGeometry gm){ this->vaoHandle = gm.vaoHandle; return *this; };
    //myGeometry(float* , int, float* , int); // verts and UVs
};

GLuint gen_geometry(float* vert_array, int vert_array_size, float *uv_array = nullptr, int uv_array_size = 0 ,float *normal_array = nullptr, int normal_array_size = 0);
GLuint gen_geometry_with_color(float* vert_array, int vert_array_size, float *color_array, int color_array_size, float *normal_array, int normal_array_size);
GLuint gen_geometry_with_color(float* vert_array, int vert_array_size, float *color_array, int color_array_size);
GLuint gen_geometry_only_vert(float* vert_array, int vert_array_size);

bool test_add_buffer_to_existing_geometry(GLuint* vaoHandle, unsigned int pos, float* values, int buff_size, int offset);
