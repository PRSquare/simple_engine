#pragma once
#include <GL/glew.h>
#include <fstream>
#include "shader_prog.h"
#include "mygeometry.h"
#include <vector>

class texture
{
    GLuint textureID;
    std::ofstream log_file;
public:
    texture( const char* );
    texture( const texture& );
    ~texture(){ log_file.close(); }
    void bind_texture(){ glBindTexture( GL_TEXTURE_2D, textureID); };
};

class objModel
{
    std::ofstream log_file;
public:
    float* verticies_array, *UV_cords_array, *normals_array;
    int vaSize, uvcaSize, naSize;
    objModel( const char* path, bool dump_obj_data = false );
    //objModel( const char* path, bool t, bool d );

    ~objModel(){ log_file.close(); delete [] verticies_array; delete [] UV_cords_array; delete [] normals_array; }
};

class vertRV_N
{
public:
    std::vector<float> vCords, nCords;
    std::vector<float> operator[](int i)
    {
        std::vector<float> retVec;
        retVec.push_back(vCords[i]);
        retVec.push_back(nCords[i]);
        return retVec;
    }
};

class hitbox
{
    std::vector<vertRV_N> hbs;
    std::ofstream log_file;
public:
    int hbCount;
    hitbox( const char* path, bool dump_obj_data = false );
    vertRV_N operator[](int i){ return hbs[i]; }
};

class text
{
    GLuint uv_offset, pos_offset, scale, textColor;
    shader_prog *shader;
    texture* tex;
    std::string str;
    float x, y, fSize;
    float rc=0.0, gc=0.0, bc=0.0, ac=1.0;
    float cr[18] =
    { -1.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0 };

    float uv[18] =
    { 0.0, 0.0, 0.0, 0.1, 0.1, 0.1, 0.0, 0.0, 0.1, 0.1, 0.1, 0.0 };
    myGeometry *letter;
    int strSize;
public:
    text(shader_prog *, texture*, std::string, int max_str_size = 40, float offset_x = -0.95, float offset_y = 0.95, float text_size = 0.05);
    void set_string(std::string text) { str = text; }
    template<typename T>
    void add_to_string(T val){ str += std::to_string(val); }
    void add_to_string(std::string val){ str += val; }
    void add_to_string(const char* val){ str += val; }

    void clear_string(){ str = ""; }

    void set_text_color(float r, float g, float b, float a) { rc = r; gc = g; bc = b; ac = a; glUniform4f(textColor, rc, bc, gc, ac); }
    void resize(float text_size) { fSize = text_size; glUniform1f(scale, fSize); }
    void move(float posX, float posY) { x = posX; y = posY; }
    void set_max_str_size(int max_str_size){ strSize = max_str_size; }
    void draw();

    template<typename T>
    void operator<<(T val){ add_to_string(val); }

};
