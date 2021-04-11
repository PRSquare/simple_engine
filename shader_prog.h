#ifndef SHADER_PROG_H
#define SHADER_PROG_H
#define GLEW_STATIC

#pragma once
#include <GL/glew.h>
#include <fstream>
#include <map>

constexpr uint ME_UNIFORM_TYPE_INT = 0;
constexpr uint ME_UNIFORM_TYPE_FLOAT = 1;

constexpr uint ME_UNIFORM_TYPE_VEC2 = 2;
constexpr uint ME_UNIFORM_TYPE_VEC3 = 3;
constexpr uint ME_UNIFORM_TYPE_VEC4 = 4;

constexpr uint ME_UNIFORM_TYPE_MAT2 = 22;
constexpr uint ME_UNIFORM_TYPE_MAT2X2 = ME_UNIFORM_TYPE_MAT2;
constexpr uint ME_UNIFORM_TYPE_MAT2X3 = 23;
constexpr uint ME_UNIFORM_TYPE_MAT2X4 = 24;

constexpr uint ME_UNIFORM_TYPE_MAT3X2 = 32;
constexpr uint ME_UNIFORM_TYPE_MAT3 = 33;
constexpr uint ME_UNIFORM_TYPE_MAT3X3 = ME_UNIFORM_TYPE_MAT3;
constexpr uint ME_UNIFORM_TYPE_MAT3X4 = 34;

constexpr uint ME_UNIFORM_TYPE_MAT4X2 = 42;
constexpr uint ME_UNIFORM_TYPE_MAT4X3 = 43;
constexpr uint ME_UNIFORM_TYPE_MAT4 = 44;
constexpr uint ME_UNIFORM_TYPE_MAT4X4 = ME_UNIFORM_TYPE_MAT4;

class shader_base
{
protected:
    bool dump_code;
    std::ofstream log_file;
    std::map<std::string, GLuint> uniforms;

    char *parsShd(const char *, const int);
    void compile_shader(const GLchar * , GLuint* );
    void link();
    void link_program(int progs_count, GLuint pr, ...);
public:
    GLuint shader_program;
    void get_uniform(const char *name)
    {
        if(uniforms.find(name) != uniforms.end() )
        {
            log_file << "WARNING! Trying to get uniform location, that already existing." << std::endl;
            log_file << "Name: " << name << std::endl;
            return;
        }
        use_program();
        GLuint val = glGetUniformLocation( shader_program, name );
        log_file << "val: " << val << std::endl;
        uniforms.insert( {name, val} );
    }
    void set_uniform_value(std::string name, uint type, void *value);
    void use_program()
    {
            glUseProgram( shader_program );
    };
};

class compute_shader : public shader_base
{
private:
    const GLchar *compute_shader_code = nullptr;
    GLuint cShader;

public:
    compute_shader(const char* path, unsigned code_length = 1024);
    ~compute_shader();
};

class shader_prog : public shader_base
{
    const GLchar *vertex_shader_code = nullptr;
    const GLchar *fragment_shader_code = nullptr;
    const GLchar *tesselation_shader_code = nullptr;
    const GLchar *geometry_shader_code = nullptr;

    GLuint vertex_shader, fragment_shader, geometry_shader, tesselation_shader;

public:
    shader_prog(unsigned code_length, std::string format, const char *path, ...);
    ~shader_prog();
    void attach(unsigned code_length, std::string format, const char *path, ...);
};

#endif // SHADER_PROG_H
