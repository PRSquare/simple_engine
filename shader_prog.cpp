#include "shader_prog.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdarg>

///++++++++++++++++++++++++++++++++++++++++++ PARSING_SHADER +++++++++++++++++++++++++++++++++++++++
char* shader_base::parsShd(const char *path, const int code_size)
{
    std::ifstream fin;
    fin.open(path);                                                      // Creating and opening file
    char* buffer_to_return = new char [code_size];
    char* buff = buffer_to_return;                                      // Creating return buffer and iterator for it

    if(!fin)
    {
        log_file << "ERROR! Can\'t open " << path << " file!" << std::endl;
    }
    while( !fin.eof() )                                                       // Parsing code from file
    {
        *buff = fin.get();
        ++buff;
    }
    *buff = '\0';

    log_file << path << ":" << std::endl;                               // Output to console ( TODO: output to log file )
    int max_len = 0;
    int now_len = 0;
    for(size_t i = 0; i < strlen(buffer_to_return); ++i)
    {
        now_len++;
        if(buffer_to_return[i] == '\n')
        {
            if( max_len < now_len )
                max_len = now_len;
            now_len = 0;
        }
    }
    buffer_to_return[strlen(buffer_to_return)-1] = '\0';
    if(dump_code)
    {
        log_file << " ";
        for(int i = 0; i < max_len + 1; ++i)
        {
            log_file << "_";
        }
        log_file << std::endl;

        bool new_str = 1;
        now_len = 0;
        for(size_t i = 0; i < std::strlen(buffer_to_return); ++i)
        {
            now_len++;
            if(new_str == 1)
            {
                log_file << "| ";
                new_str = 0;
            }
            if(buffer_to_return[i] == '\n')
            {
                for(int j = 0; j < max_len - now_len; ++j)
                    log_file << " ";
                log_file << " |";
                new_str = 1;
                now_len = 0;
            }
            log_file << buffer_to_return[i];
        }
        for(int j = 0; j < max_len-1; ++j)
                    log_file << " ";
        log_file << "|" << std::endl << " ";
        for(int i = 0; i < max_len + 1; ++i)
        {
            log_file << "_";
        }
        log_file << std::endl << std::endl;
    }
    //std::cout << strlen(buffer_to_return) << std::endl;
    fin.close();                                                        // Closing file

    return buffer_to_return;                                            // Returning buffer ( code as string )
}


void shader_base::compile_shader(const GLchar* shader_code, GLuint* shader_to_compile)
{
    const GLchar* code_array[] = { shader_code };

    glShaderSource( *shader_to_compile, 1, code_array, NULL );
    glCompileShader( *shader_to_compile );
    GLint result;
    glGetShaderiv( *shader_to_compile, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result )
    {
        log_file << "ERROR! Shader compilation failed!" << std::endl;
        GLint logLen;
        glGetShaderiv( *shader_to_compile, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 )
        {
            char* log = new char[logLen];

            GLsizei written;
            glGetShaderInfoLog( *shader_to_compile, logLen, &written, log );
            log_file << "Shader log: " << log << std::endl;
            delete [] log;
        }
    }
}

void shader_base::link()
{
    glLinkProgram( shader_program );

    GLint status;
    glGetProgramiv( shader_program, GL_LINK_STATUS, &status );
    if( GL_FALSE == status )
    {
        log_file << "ERROR! Failed to link shader program" << std::endl;
    }
    GLint logLen;
    glGetProgramiv( shader_program, GL_INFO_LOG_LENGTH, &logLen );
    if(logLen > 0)
    {
        GLchar* log = new GLchar[logLen];

        GLsizei written;

        glGetProgramInfoLog( shader_program, logLen, &written, log );
        log_file << log << std::endl;

        delete [] log;
    }
}

void shader_base::link_program(int progs_count, GLuint pr1, ...)
{
    shader_program = glCreateProgram();
    if( 0 == shader_program )
    {
        log_file << "ERROR! Can't create shader program" << std::endl;
    }
    va_list shaders;
    va_start(shaders, pr1);
    glAttachShader(shader_program, pr1);
    for (int i = 1; i < progs_count; ++i)
    {
        GLuint curProg = va_arg(shaders, GLuint);
        glAttachShader(shader_program, curProg);
    }
    va_end(shaders);
    link();
}


void shader_base::set_uniform_value(std::string name, uint type, void *value)
{
    if(uniforms.find(name) == uniforms.end())
    {
        get_uniform(name.c_str());
    }
    glUseProgram(shader_program);
    switch (type)
    {
    case ME_UNIFORM_TYPE_INT:
    {
        int *val = static_cast<int*>(value);
        glUniform1i(uniforms[name], *val);
        break;
    }
    case ME_UNIFORM_TYPE_FLOAT:
    {
        float *val = static_cast<float*>(value);
        glUniform1f(uniforms[name], *val);
        break;
    }
    case ME_UNIFORM_TYPE_VEC2:
    {
        float *val = static_cast<float*>(value);
        glUniform2f(uniforms[name], val[0], val[1]);
        break;
    }
    case ME_UNIFORM_TYPE_VEC3:
    {
        float *val = static_cast<float*>(value);
        glUniform3f(uniforms[name], val[0], val[1], val[2]);
        break;
    }
    case ME_UNIFORM_TYPE_VEC4:
    {
        float *val = static_cast<float*>(value);
        glUniform4f(uniforms[name], val[0], val[1], val[2], val[3]);
        break;
    }
    case ME_UNIFORM_TYPE_MAT2:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix2fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT2X3:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix2x3fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT2X4:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix2x4fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT3X2:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix3x2fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT3:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix3fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT3X4:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix3x4fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT4X2:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix4x2fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT4X3:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix4x3fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    case ME_UNIFORM_TYPE_MAT4:
    {
        float *val = static_cast<float*>(value);
        glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, val);
        break;
    }
    default:
        log_file << "WARNING! Unnknown uniform type! Uniform value was not changed." << std::endl;
    }
}


compute_shader::compute_shader(const char *path, unsigned code_length)
{
    log_file.open("log.txt", std::ios_base::app);
    compute_shader_code = parsShd(path, code_length);
    cShader = glCreateShader( GL_COMPUTE_SHADER );
    compile_shader(compute_shader_code, &cShader);
    link_program(1, cShader);
    log_file << "Compute shader " << cShader << " was created." << std::endl;
}

compute_shader::~compute_shader()
{
    delete [] compute_shader_code;
    log_file << "Compute shader " << cShader << " was  destroyed." << std::endl;
    log_file.close();

}

shader_prog::shader_prog(unsigned code_length, std::string format, const char *path, ...)
{
    log_file.open("log.txt", std::ios_base::app);
    va_list paths;
    va_start(paths, &path);
    bool taken = false;
    for(auto &a : format)
    {
        switch (a)
        {
        case 'v':
            // Vertex shader
            vertex_shader = glCreateShader( GL_VERTEX_SHADER );
            if( vertex_shader == 0)
            {
                log_file << "ERROR! Can't create vertex shader!" << std::endl;
                return;
            }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            vertex_shader_code = parsShd( path, code_length);
            compile_shader( vertex_shader_code, &vertex_shader );
            continue;
        case 'f':
            // Fragment shader
            fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
            if( fragment_shader == 0)
            {
                log_file << "ERROR! Can't create fragment shader!" << std::endl;
                return;
            }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            fragment_shader_code = parsShd( path, code_length);
            compile_shader( fragment_shader_code, &fragment_shader );
            continue;
        case 't':
            // Tessilation sgader
            tesselation_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
            if( tesselation_shader == 0 )
            {
                log_file << "ERROR! Can't create tesselation shader!" << std::endl;
             }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            tesselation_shader_code = parsShd( path, code_length);
            compile_shader(tesselation_shader_code, &tesselation_shader);
            continue;
        case 'g':
            // Geometry shader
            glCreateShader( GL_GEOMETRY_SHADER );
            if( geometry_shader == 0 )
            {
                log_file << "ERROR! Can't create tesselation shader!" << std::endl;
            }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            geometry_shader_code = parsShd( path, code_length);
            compile_shader(geometry_shader_code, &geometry_shader);
            continue;
        case ' ':
            continue;
        case '\0':
            continue;
        default:
            log_file << "WARNING! Unknown parameter \'" << a << "\' used in process of shader compiling." << std::endl;
            continue;
        }
    }
    va_end(paths);
    if(vertex_shader_code == nullptr || fragment_shader_code == nullptr)
    {
        log_file << "ERROR! Shader program wasn't created.\nIt must contains at least Vertex and Fragment shaders.";
        return;
    }
    if(tesselation_shader_code != nullptr || geometry_shader_code != nullptr)
    {
        log_file << "Full shader pipeline" << std::endl;
        link_program(4, vertex_shader, tesselation_shader,  geometry_shader, fragment_shader);
    }
    else
        link_program(2, vertex_shader, fragment_shader);
    log_file << "Shader program " << shader_program << " created" << std::endl;
}
///++++++++++++++++++++++++++++++++++++++++++ DESTRUCTOR +++++++++++++++++++++++++++++++++++++++++++
shader_prog::~shader_prog()
{
    glDeleteProgram( shader_program );
    delete [] vertex_shader_code;                                       // Removing vars with shader code
    delete [] fragment_shader_code;
    if(tesselation_shader_code != nullptr)
        delete [] tesselation_shader_code;
    if(geometry_shader_code != nullptr)
        delete [] geometry_shader_code;
    log_file << "Shader program " << shader_program << " was destroyed." << std::endl;
    log_file.close();
}

void shader_prog::attach(unsigned int code_length, std::string format, const char *path, ...)
{
    va_list paths;
    va_start(paths, &path);
    bool taken = false;
    for(auto &a : format)
    {
        switch (a)
        {
        case 't':
            // Tessilation sgader
            tesselation_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
            if( tesselation_shader == 0 )
            {
                log_file << "ERROR! Can't create tesselation shader!" << std::endl;
             }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            tesselation_shader_code = parsShd( path, code_length);
            compile_shader(tesselation_shader_code, &tesselation_shader);
            continue;
        case 'g':
            // Geometry shader
            glCreateShader( GL_GEOMETRY_SHADER );
            if( geometry_shader == 0 )
            {
                log_file << "ERROR! Can't create tesselation shader!" << std::endl;
            }
            if(taken)
                path = va_arg(paths, GLchar*);
            else
                taken = true;
            geometry_shader_code = parsShd( path, code_length);
            compile_shader(geometry_shader_code, &geometry_shader);
            continue;
        case ' ':
            continue;
        case '\0':
            continue;
        default:
            log_file << "WARNING! Unknown parameter \'" << a << "\' used in process of shader compiling." << std::endl;
            continue;
        }
    }
    va_end(paths);
    if(tesselation_shader_code != nullptr)
    {
        glAttachShader(shader_program, tesselation_shader);
        link();
    }
    if( geometry_shader_code != nullptr )
    {
        glAttachShader(shader_program, geometry_shader);
        link();
    }
    log_file << "Shader attached" << std::endl;

}
