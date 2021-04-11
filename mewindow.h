#ifndef MEWINDOW_H
#define MEWINDOW_H
#include <GLFW/glfw3.h>
#include <fstream>

class meWindow
{
protected:
    virtual void inicialization(unsigned short gl_version_maj, unsigned short gl_version_min);
    virtual void destroy();
    std::ofstream log_file;
public:
    unsigned width, height;
    const char *name;
    GLFWwindow *window;
    meWindow(unsigned window_width, unsigned window_height, const char *window_name, unsigned short gl_version_maj = 4, unsigned short gl_version_min = 3, const char *log_file_path = nullptr)
    {
        if (log_file_path != nullptr)
        {
            log_file.open(log_file_path, std::ios_base::app);
        }
        width = window_width;
        height = window_height;
        name = window_name;
        inicialization(gl_version_maj, gl_version_min);
    }
    void log(const char *path)
    {
        log_file.open(path, std::ios_base::app);
    }


    virtual void draw();

    virtual ~meWindow()
    {
        destroy();
        if (log_file.is_open())
            log_file.close();
    }
};

#endif // MEWINDOW_H
