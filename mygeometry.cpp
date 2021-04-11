#include "mygeometry.h"

myGeometry::myGeometry( float* vert_array, int vert_array_size, float *uv_array, int uv_array_size, float *normal_array, int normal_array_size )
{
    GLuint vboHandles[3];
    glGenBuffers( 3, vboHandles );
    GLuint positionBufferHandle = vboHandles[0];
    GLuint uvBufferHandle = vboHandles[1];
    GLuint normalBufferHandle = vboHandles[2];

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, vert_array_size, vert_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, uv_array_size, uv_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, normal_array_size, normal_array, GL_DYNAMIC_DRAW );

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray( vaoHandle );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferHandle );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );

}

myGeometry::myGeometry( const myGeometry & mgCopy )
{
    vaoHandle = mgCopy.vaoHandle;
}

GLuint gen_geometry(float* vert_array, int vert_array_size, float *uv_array, int uv_array_size, float *normal_array, int normal_array_size)
{
    GLuint vaoHandle;
    GLuint vboHandles[3];
    glGenBuffers( 3, vboHandles );
    GLuint positionBufferHandle = vboHandles[0];
    GLuint uvBufferHandle = vboHandles[1];
    GLuint normalBufferHandle = vboHandles[2];

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, vert_array_size, vert_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, uv_array_size, uv_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, normal_array_size, normal_array, GL_DYNAMIC_DRAW );

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray( vaoHandle );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferHandle );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    return vaoHandle;
}

GLuint gen_geometry_with_color(float* vert_array, int vert_array_size, float *color_array, int color_array_size, float *normal_array, int normal_array_size)
{
    GLuint vaoHandle;
    GLuint vboHandles[3];
    glGenBuffers( 3, vboHandles );
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];
    GLuint normalBufferHandle = vboHandles[2];

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, vert_array_size, vert_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, colorBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, color_array_size, color_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, normal_array_size, normal_array, GL_DYNAMIC_DRAW );

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray( vaoHandle );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, colorBufferHandle );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, normalBufferHandle );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    return vaoHandle;
}

GLuint gen_geometry_with_color(float* vert_array, int vert_array_size, float *color_array, int color_array_size )
{
    GLuint vaoHandle;
    GLuint vboHandles[2];
    glGenBuffers( 2, vboHandles );
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, vert_array_size, vert_array, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, colorBufferHandle );
    glBufferData( GL_ARRAY_BUFFER, color_array_size, color_array, GL_DYNAMIC_DRAW );

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray( vaoHandle );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer( GL_ARRAY_BUFFER, positionBufferHandle );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    glBindBuffer( GL_ARRAY_BUFFER, colorBufferHandle );
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, NULL );

    return vaoHandle;
}

GLuint gen_geometry_only_vert(float* vert_array, int vert_array_size )
{
    GLuint vaoHandle;
    GLuint vboHandles;
    glGenBuffers( 1, &vboHandles );

    glBindBuffer( GL_ARRAY_BUFFER, vboHandles );
    glBufferData( GL_ARRAY_BUFFER, vert_array_size, vert_array, GL_DYNAMIC_DRAW );

    glGenVertexArrays( 1, &vaoHandle );
    glBindVertexArray( vaoHandle );

    glEnableVertexAttribArray(0);

    glBindBuffer( GL_ARRAY_BUFFER, vboHandles );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    return vaoHandle;
}


bool test_add_buffer_to_existing_geometry(GLuint* vaoHandle, unsigned int pos, float* values, int buff_size, int offset)
{
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
    glBufferData( GL_ARRAY_BUFFER, buff_size, values, GL_DYNAMIC_DRAW );

    glBindVertexArray( *vaoHandle );

    glEnableVertexAttribArray(pos);
    glBindBuffer( GL_ARRAY_BUFFER, vboHandle );
    glVertexAttribPointer( pos, offset, GL_FLOAT, GL_FALSE, 0, NULL );

    return true;
}

