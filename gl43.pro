TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lOpenGL -lglfw -lGLEW

SOURCES += \
        figure.cpp \
        graph.cpp \
        main.cpp \
        mewindow.cpp \
        mygeometry.cpp \
        shader_prog.cpp

DISTFILES += \
    shaders/anim_obj_shader/fragment.glsl \
    shaders/anim_obj_shader/vertex.glsl \
    shaders/compute_shaders/mv_shader.glsl \
    shaders/debug_shaders/col_only_frag.glsl \
    shaders/debug_shaders/col_only_vert.glsl \
    shaders/mainFragment.glsl \
    shaders/mainVertex.glsl \
    shaders/pointFragment.glsl \
    shaders/pointVertex.glsl \
    shaders/test.glsl \
    shaders/textFragment.glsl \
    shaders/textVertex.glsl

HEADERS += \
    figure.h \
    graph.h \
    mewindow.h \
    mygeometry.h \
    shader_prog.h
