#include <GL/glew.h>
#include "mewindow.h"

void meWindow::inicialization(unsigned short gl_version_maj, unsigned short gl_version_min)
{
    if (!glfwInit())
    {
        if(log_file.is_open())
            log_file << "ERROR! Can't inicialize GLFW." << std::endl;
        return;
    }

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window)
    {
        if(log_file.is_open())
            log_file << "ERROR! Can't create window." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);


    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_maj);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version_min);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLenum glew_init_status = glewInit();
    if(glew_init_status != GLEW_OK)
    {
        if(log_file.is_open())
            log_file << "!!! ERROR! Failed to inicialise GLEW:\n" << glewGetErrorString( glew_init_status ) << std::endl;
    }

    if (GLEW_VERSION_4_3)
    {
        if(log_file.is_open())
            log_file << "OpenGL 4.3 is suported!" << std::endl;
    }
}

void meWindow::draw()
{
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void meWindow::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    if(log_file.is_open())
        log_file << "Window terminated";
}
