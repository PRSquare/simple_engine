#include "figure.h"
#include <glm/gtc/matrix_transform.hpp>
#include <strstream>

figure::figure(GLuint obj_handle, int figure_size, shader_prog* shader, texture* fig_texture, float posX, float posY, float posZ, float scaleV, float rotationX, float rotationY, float rotationZ )
{
    vao_handle = obj_handle;
    figure_shader = shader;
    translationMatrix = rotationMatrix = scaleMatrix = modelMatrix = glm::mat4{1.0};
    translationMatrix = glm::translate( glm::mat4(1.0), glm::vec3(posX, posY, posZ) );
    rotationMatrix = glm::rotate( glm::mat4(1.0), rotationX, xAxis );
    rotationMatrix = glm::rotate( glm::mat4(1.0), rotationY, yAxis );
    rotationMatrix = glm::rotate( glm::mat4(1.0), rotationZ, zAxis );
    scaleMatrix = glm::scale( glm::mat4(1.0), glm::vec3(scaleV, scaleV, scaleV) );
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    //modelMatrix = glm::mat4{1.0};

    figure_texture = fig_texture;
    fig_size = figure_size;
    glsl_model_matrix_name = "modelMatrix";
    modMatrix = glGetUniformLocation( figure_shader->shader_program, glsl_model_matrix_name.c_str() );
    rotMatrix = glGetUniformLocation( figure_shader->shader_program, "rotationMatrix" );
    figure_shader->use_program();
    glUniformMatrix4fv( modMatrix, 1, GL_FALSE, &modelMatrix[0][0] );
}

void figure::update()
{
    if(mat_is_changed)
    {
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        if(box_cords_min != box_cords_max)
        {
            modificated_box_cords_min = translationMatrix * scaleMatrix * box_cords_min;
            modificated_box_cords_max = translationMatrix * scaleMatrix * box_cords_max;
        }
    }

//    std::cout << vao_handle << std::endl;
//    std::cout << box_cords_min.x << " " << box_cords_min.y << " " << box_cords_min.z << std::endl << std::endl;
//    for(int j = 0; j < 4; ++j)
//    {
//        for(int i = 0; i < 4; ++i)
//        {
//            std::cout << modelMatrix[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << "___________________" << std::endl;

}

void figure::draw()
{
    update();
    figure_shader->use_program();
    glUniformMatrix4fv( modMatrix, 1, GL_FALSE, &modelMatrix[0][0] );
    glUniformMatrix4fv( rotMatrix, 1, GL_FALSE, &rotationMatrix[0][0] );

    if(figure_texture != nullptr)
        figure_texture->bind_texture();
    glBindVertexArray( vao_handle );
    glDrawArrays( draw_mode, 0, fig_size );

}

void figure::set_drawig_mode(GLuint drawing_mode)
{
    draw_mode = drawing_mode;
}

void figure::move(float x, float y, float z)
{
    translationMatrix = glm::translate( glm::mat4(1.0), glm::vec3(x, y, z) );
    mat_is_changed = true;
}
void figure::rotate(float x_rotation, float y_rotation, float z_rotation)
{
    rotationMatrix = glm::mat4(1.0);
    rotationMatrix *= glm::rotate( glm::mat4(1.0), x_rotation, xAxis );
    rotationMatrix *= glm::rotate( glm::mat4(1.0), y_rotation, yAxis );
    rotationMatrix *= glm::rotate( glm::mat4(1.0), z_rotation, zAxis );
    mat_is_changed = true;
}
void figure::scale(float scale_value)
{
    scaleMatrix = glm::scale( glm::mat4(1.0), glm::vec3(scale_value, scale_value, scale_value) );
    mat_is_changed = true;
}

void figure::scale(float x_scale, float y_scale, float z_scale)
{
    scaleMatrix = glm::scale( glm::mat4(1.0), glm::vec3(x_scale, y_scale, z_scale) );
    mat_is_changed = true;
}

void figure::set_shader_program(shader_prog *new_shader)
{
    figure_shader = new_shader;
}

void figure::set_object_box(float *cords)
{
    for(int i = 0; i < 3; ++i)
        box_cords_min[i] = cords[i];
    for(int i = 0; i < 3; ++i)
        box_cords_max[i] = cords[i+3];
    modificated_box_cords_min = translationMatrix * scaleMatrix * box_cords_min;
    modificated_box_cords_max = translationMatrix * scaleMatrix * box_cords_max;
}

void figure::set_object_box(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    box_cords_min.x = minX; box_cords_min.y = minY; box_cords_min.z = minZ;
    box_cords_max.x = maxX; box_cords_max.y = maxY; box_cords_max.z = maxZ;
    modificated_box_cords_min = translationMatrix * scaleMatrix * box_cords_min;
    modificated_box_cords_max = translationMatrix * scaleMatrix * box_cords_max;
}

#include <iostream>
void figure::set_object_box( const char* filename )
{
    std::ifstream file(filename);
    if(!file.is_open())
        return;

    float   minX = MAXFLOAT, maxX,
            minY, maxY,
            minZ, maxZ;
    float nx, ny, nz;
    std::string sIgnore;
    while (!file.eof())
    {
        char line[128];
        std::strstream lnStr;
        file.getline(line, 128);
        lnStr << line;
        if(line[0] == 'v')
        {
            lnStr >> sIgnore >> nx >> ny >> nz;

            if(minX == MAXFLOAT)
            {
                    minX = maxX = nx;
                    minY = maxY = ny;
                    minZ = maxZ = nz;
            }
            else
            {
                if(nx < minX)
                    minX = nx;
                if(nx > maxX)
                    maxX = nx;
                if(ny < minY)
                    minY = ny;
                if(ny > maxY)
                    maxY = ny;
                if(nz < minZ)
                    minZ = nz;
                if(nz > maxZ)
                    maxZ = nz;
            }
        }
        if(line[0] == 's')
            break;
    }
    box_cords_min.x = minX; box_cords_min.y = minY; box_cords_min.z = minZ;
    box_cords_max.x = maxX; box_cords_max.y = maxY; box_cords_max.z = maxZ;
    modificated_box_cords_min = translationMatrix * scaleMatrix * box_cords_min;
    modificated_box_cords_max = translationMatrix * scaleMatrix * box_cords_max;
}

bool figure::collision(float x, float y, float z)
{
    glm::vec3 pos(x, y, z);

    glm::vec4 xAsix(1.0, 0.0, 0.0, 0.0);
    glm::vec4 yAsix(0.0, 1.0, 0.0, 0.0);
    glm::vec4 zAsix(0.0, 0.0, 1.0, 0.0);
    //translationMatrix * rotationMatrix * scaleMatrix
    xAsix = translationMatrix * rotationMatrix*xAsix;
    yAsix = translationMatrix * rotationMatrix*yAsix;
    zAsix = translationMatrix * rotationMatrix*zAsix;

    glm::mat3 prMat(xAsix.x, xAsix.y, xAsix.z,
              yAsix.x, yAsix.y, yAsix.z,
              zAsix.x, zAsix.y, zAsix.z);
    pos = prMat * pos;

//    a11 a12 a13
//    a21 a22 a23
//    a31 a32 a33

//    if(x > modificated_box_cords_min.x && x < modificated_box_cords_max.x)
//        if( y > modificated_box_cords_min.y && y < modificated_box_cords_max.y )
//            if( z > modificated_box_cords_min.z && z < modificated_box_cords_max.z )
//                return true;
//    return false;

    std::cout << pos.x << " " << pos.y << " " << pos.z << "\r" << std::flush;

    if(pos.x > box_cords_min.x && pos.x < box_cords_max.x)
        if( pos.y > box_cords_min.y && pos.y < box_cords_max.y )
            if( pos.z > box_cords_min.z && pos.z < box_cords_max.z )
                return true;
    return false;
}


void figure::draw_in_proj(GLuint vhandle, int size, texture *tex, int mode)
{
    figure_shader->use_program();
    glUniformMatrix4fv( modMatrix, 1, GL_FALSE, &modelMatrix[0][0] );

    tex->bind_texture();
    glBindVertexArray( vhandle );
    glDrawArrays( mode, 0, size );
}

void figure::draw_in_proj(GLuint vhandle, int size, shader_prog *sh, int mode)
{
    sh->use_program();
    GLuint mmatr = glGetUniformLocation(sh->shader_program, "modelMatrix");
    glUniformMatrix4fv( mmatr, 1, GL_FALSE, &modelMatrix[0][0] );

    glBindVertexArray( vhandle );
    glDrawArrays( mode, 0, size );


}






