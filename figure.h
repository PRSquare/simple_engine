#pragma once
#include "graph.h"
#include <glm/glm.hpp>

class figure{
    glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
    shader_prog* figure_shader;
    GLuint vao_handle;
    glm::vec3 xAxis = glm::vec3(1.0, 0.0, 0.0), yAxis = glm::vec3(0.0, 1.0, 0.0), zAxis = glm::vec3(0.0, 0.0, 1.0);
    texture* figure_texture;
    GLuint draw_mode = GL_TRIANGLES;
    GLuint modMatrix, rotMatrix;
    std::string glsl_model_matrix_name;
    int fig_size;
    glm::vec4 box_cords_min = glm::vec4{0, 0, 0, 1};
    glm::vec4 box_cords_max = glm::vec4{0, 0, 0, 1};
    glm::vec4 modificated_box_cords_min = glm::vec4{0, 0, 0, 1};
    glm::vec4 modificated_box_cords_max = glm::vec4{0, 0, 0, 1};

    bool mat_is_changed = false;

    void update();
public:
    glm::mat4 modelMatrix;
    figure(GLuint obj_handle, int figure_size, shader_prog* shader, texture* fig_texture = nullptr, float posX = 0.0, float posY = 0.0,
           float posZ = 0.0, float scale = 1.0, float rotationX = 0.0, float rotationY = 0.0, float rotationZ = 0.0 );
    void draw();
    void set_drawig_mode(GLuint drawing_mode);
    void move(float x, float y, float z);
    void rotate(float x_rotation, float y_rotation, float z_rotation);
    void scale(float scale_value);
    void scale(float x_scale, float y_scale, float z_scale);
    void set_shader_program(shader_prog* new_shader);
    void set_mod_matrix_name(std::string name){ glsl_model_matrix_name = name; }
    void set_object_box( float cords[6]);
    void set_object_box( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );
    void set_object_box( const char* filename );
    bool collision(float x, float y, float z);

    void draw_in_proj( GLuint vhandle, int size, texture* tex, int mode = GL_POINTS );
    void draw_in_proj( GLuint vhandle, int size, shader_prog* sh, int mode = GL_POINTS );
};
