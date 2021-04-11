#include <iostream>
#include "figure.h"
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include <mewindow.h>
#include <math.h>

constexpr float PI = 3.14159265359;

std::ofstream log_file;

struct vertWB
{
    std::vector<int> boneIDs;
    std::vector<float> vWeights;
};

// Функция для парсинга joint'ов
void pars_skeleton(std::string path, std::vector<float> *rig_mats, std::vector<vertWB> *thisJoints )
{
    std::ifstream in_file(path);
    std::vector<float> weights;
    std::vector<int> jCount;

    std::string curVal;
    in_file >> curVal;
    while (curVal != "!")
    {
        weights.push_back(std::atof( curVal.c_str() ));
        in_file >> curVal;
    }

    in_file >> curVal;
    while (curVal != "!")
    {
        rig_mats->push_back(std::atof( curVal.c_str() ));
        in_file >> curVal;
    }

    in_file >> curVal;
    while (curVal != "!")
    {
        jCount.push_back(std::atof( curVal.c_str() ));
        in_file >> curVal;
    }

    for(auto &i : jCount)
    {
        vertWB cVert;
        for(int j = 0; j < i; ++j)
        {
            int b_id, w_id;
            in_file >> curVal;
            b_id = std::atoi( curVal.c_str() );

            in_file >> curVal;
            w_id = std::atoi( curVal.c_str() );

            cVert.boneIDs.push_back( b_id );
            cVert.vWeights.push_back( weights[w_id] );
        }
        thisJoints->push_back(cVert);
    }

    in_file.close();

}

//
myGeometry geomInicial(objModel* object)
{
    myGeometry geom( object->verticies_array, object->vaSize*sizeof(float), object->UV_cords_array, object->uvcaSize*sizeof(float), object->normals_array, object->naSize*sizeof(float));
    return geom;
}

int main(void)
{
    std::cout << "Hello world";
    log_file.open("log.txt");
    if( !log_file.is_open() )
    {
        return -1;
    }
    meWindow testWindow(1280, 960, "KeK");
    testWindow.log("log.txt");


//_________________________________________________________________________________________________________________________________________________
    // Загрузка шейдеров
    //shader_prog myShader("../gl43/shaders/mainVertex.glsl", "../gl43/shaders/mainFragment.glsl");
    shader_prog myShader(2048, "v g f", "../gl43/shaders/mainVertex.glsl", "../gl43/shaders/test.glsl", "../gl43/shaders/mainFragment.glsl");
    shader_prog myShader2(2048, "v f", "../gl43/shaders/textVertex.glsl", "../gl43/shaders/textFragment.glsl");
    shader_prog centerPoinShader(1024, "v f", "../gl43/shaders/pointVertex.glsl", "../gl43/shaders/pointFragment.glsl");

    //myShader.attach(1024, "g", "../gl43/shaders/test.glsl");


    compute_shader cTestShader("../gl43/shaders/compute_shaders/mv_shader.glsl");

    shader_prog animShader(2048, "v f", "../gl43/shaders/anim_obj_shader/vertex.glsl", "../gl43/shaders/anim_obj_shader/fragment.glsl", 2048);

    // Загрузка текстур
    texture solidTex("../gl43/img/solidTex.dds"); // Градиентная текстура
    texture transparentTex("../gl43/img/transparentTex.dds");// Полупрозрачная текстура

    // Дерево
    objModel treeObj("../gl43/obj/env/tree.obj");
    texture treeTex( "../gl43/img/env/tree.dds" );
    myGeometry treeGeom = geomInicial( &treeObj );
    figure tree( treeGeom.vaoHandle, treeObj.vaSize, &myShader, &treeTex, -3.0 );

    // Дом
    objModel houseObj("../gl43/obj/env/house.obj");
    texture houseTex( "../gl43/img/env/house.dds" );
    myGeometry houseGeom = geomInicial( &houseObj );
    figure house( houseGeom.vaoHandle, houseObj.vaSize, &myShader, &houseTex, -10.0, 0.0, 0.0, 1.5 );
    house.rotate(PI/4, 0.0, 0.0);

    // Санечек
    objModel mObj2("../gl43/obj/pers.obj");
    texture mTex2( "../gl43/img/persSkin2.dds" );

    // Голова
    objModel mObj("../gl43/obj/head.obj");
    texture mTex( "../gl43/img/Head.dds" );

    // Текстура шрифта
    texture font("../gl43/img/font/sample-atlas.dds");

    // Поверхность
    objModel plane( "../gl43/obj/landscape.obj" );
    texture cells( "../gl43/img/landscape.dds" );

    // Создание GLUint переменных для отрисовки
    GLuint floor = gen_geometry( plane.verticies_array, plane.vaSize*sizeof(float), plane.UV_cords_array, plane.uvcaSize*sizeof(float), plane.normals_array, plane.naSize*sizeof(float) );

    GLuint headObj = gen_geometry( mObj.verticies_array, mObj.vaSize*sizeof(float), mObj.UV_cords_array, mObj.uvcaSize*sizeof(float), mObj.normals_array, mObj.naSize*sizeof (float) );


    GLuint sanya = gen_geometry_only_vert( mObj2.verticies_array, mObj2.vaSize*sizeof(float) );
    test_add_buffer_to_existing_geometry(&sanya, 1, mObj2.UV_cords_array, mObj2.uvcaSize*sizeof(float), 2);
    test_add_buffer_to_existing_geometry(&sanya, 2, mObj2.normals_array, mObj2.naSize*sizeof (float), 3);

    std::vector<float> rigs;
    std::vector<vertWB> wbs;

    pars_skeleton("../gl43/obj/pers_skeleton.txt", &rigs, &wbs);

    std::cout << mObj2.uvcaSize << " " << wbs.size()*3 << std::endl;
    float inA[wbs.size()];
    for(int i = 0; i < wbs.size(); ++i)
    {
        inA[i] = wbs[i].boneIDs.size();
    }
    test_add_buffer_to_existing_geometry(&sanya, 3, inA, sizeof (inA), 1);

    // Сборка всего в объект фигуры
    figure floorFig( floor, plane.vaSize, &myShader, &cells);
    figure myTestFig( sanya, mObj2.vaSize, &myShader, &mTex2, 5.0, 1.0, 1.0, 0.1);
    figure smthFig( headObj, mObj.vaSize, &myShader, &mTex);

    floorFig.scale(0.5); // Уменьшение размера пола

    // Получение необходимых uniform переменных
    int *locVal = new int;
    *locVal = GL_TEXTURE_2D;
    myShader.set_uniform_value("Tex1", ME_UNIFORM_TYPE_INT, &locVal);
    myShader2.set_uniform_value("Tex1", ME_UNIFORM_TYPE_INT, &locVal);
    delete locVal;

//_________________________________________________________________________________________________________________________________________________

    float lightVec[3] = {-1.0, -1.0, -1.0};
    float plPosition[3] = {0, 0, 0};

    myShader.use_program();

    float xPos = 0, yPos = 1, zPos = -5;
    glm::mat4 viewMat = glm::lookAt(glm::vec3(xPos, yPos, zPos), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    float alpha = 0, beta = 0, gamma = 0;
    glm::mat4 projMat = glm::perspective(glm::radians(90.0), 4.0/3.0, 0.1, 200.0);
    myShader.set_uniform_value("projectionMatrix", ME_UNIFORM_TYPE_MAT4, &projMat);
    myShader.set_uniform_value("lightingVec", ME_UNIFORM_TYPE_VEC3, &lightVec);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    text myText(&myShader2, &font, "");
    myText.resize(0.03);

    clock_t t = clock();
    time_t timer1, timer2;
    time(&timer1); time(&timer2);
    float updateTime, curUT = 0;
    int fps = 0, frameCount = 0;

    double mousePos[2];
    glfwSetInputMode(testWindow.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    smthFig.move(2, 3, 1);

    house.set_object_box( "../gl43/obj/hitboxes/house/househb.obj" );

    float centerPointCords[3] = {0.0, 0.0, 0.0};
    float centerPointColor[4] = {1.0, 0.0, 0.0, 0.5};
    GLuint centerPointVaoHandle =  gen_geometry_with_color(centerPointCords, sizeof(centerPointCords), centerPointColor, sizeof(centerPointColor));
    figure centerPointFigure( centerPointVaoHandle, 1, &centerPoinShader );
    centerPointFigure.set_drawig_mode( GL_POINTS );


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(testWindow.window))
    {

        // 1. Movement timer & fps
        updateTime = ((float) clock()-t )/CLOCKS_PER_SEC *1000;
        t = clock();

        time(&timer1);
        if(difftime(timer1, timer2) >= 1)
        {
            time(&timer2);
            fps = frameCount;
            curUT=updateTime;
            frameCount = 0;
        }
        frameCount++;
        // -------

        // 2. Clearing window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5, 0.5, 0.5, 1.0);
        // -------

        // 3. Drawing all figures
        floorFig.draw();
        smthFig.draw();
        tree.draw();
        house.draw();

        myTestFig.draw();
        // -------

        // 4. Mouse input
        glfwGetCursorPos(testWindow.window, &mousePos[0], &mousePos[1]);
        mousePos[0] = -1*(mousePos[0] - testWindow.width/2)/testWindow.width;
        mousePos[1] = (mousePos[1] - testWindow.height/2)/testWindow.height;
        glfwSetCursorPos(testWindow.window, testWindow.width/2, testWindow.height/2);

        if( glfwGetWindowAttrib(testWindow.window, GLFW_FOCUSED) == GLFW_TRUE )
        {
            if(beta > -PI/2.5 + 0.1 && beta < PI/2.5 - 0.1)
                beta += (mousePos[1])*updateTime;
            alpha += (mousePos[0])*updateTime;
        }
        // -------

        // 5. Keyboard input
        if(glfwGetKey(testWindow.window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if(beta > -PI/3 + 0.1)
                beta -= 0.03*updateTime;
        }

        if(glfwGetKey(testWindow.window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if(beta < PI/3 - 0.1)
                beta += 0.03*updateTime;
        }

        if(glfwGetKey(testWindow.window, GLFW_KEY_LEFT) == GLFW_PRESS)
            alpha += 0.03*updateTime;

        if(glfwGetKey(testWindow.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            alpha -= 0.03*updateTime;

        if(glfwGetKey(testWindow.window, GLFW_KEY_SPACE) == GLFW_PRESS)
            yPos += 0.03*updateTime;

        if(glfwGetKey(testWindow.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            yPos -= 0.03*updateTime;

        if(glfwGetKey(testWindow.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            xPos += 0.06*cos(alpha)*updateTime;
            zPos -= 0.06*sin(alpha)*updateTime;
        }

        if(glfwGetKey(testWindow.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            xPos -= 0.06*cos(alpha)*updateTime;
            zPos += 0.06*sin(alpha)*updateTime;
        }

        if(glfwGetKey(testWindow.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            zPos -= 0.06*cos(alpha)*updateTime;
            xPos -= 0.06*sin(alpha)*updateTime;
        }

        if(glfwGetKey(testWindow.window, GLFW_KEY_W) == GLFW_PRESS)
        {
            zPos += 0.06*cos(alpha)*updateTime;
            xPos += 0.06*sin(alpha)*updateTime;
        }
        if(glfwGetKey(testWindow.window, GLFW_KEY_R) == GLFW_PRESS)
        {
            zPos = 0.0;
            xPos = 0.0;
            yPos = 0.0;
        }
        // -------

        // 6. View matrix update
        glm::mat3 rMatX(
                    1.0, 0.0, 0.0,
                    0.0, cos(alpha), -sin(alpha),
                    0.0, sin(alpha), cos(alpha)
                    );
        glm::mat3 rMatY(
                    cos(beta), 0, sin(beta),
                    0, 1, 0,
                    -sin(beta), 0, cos(beta)
                    );
        glm::mat3 rMatZ(
                    cos(gamma), -sin(gamma), 0.0,
                    sin(gamma), cos(gamma), 0.0,
                    0.0, 0.0, 1.0
                    );

        if(alpha > 2*PI || alpha < -2*PI)
            alpha = 0;

        if(beta <= -PI/2.5 + 0.1)
            beta += 0.001;

        if(beta >= PI/2.5 - 0.1)
            beta -= 0.001;
        // -------

        // 7. Set uniforms
        viewMat = glm::lookAt(glm::vec3(xPos, yPos, zPos), glm::vec3(xPos + sin(alpha), yPos - sin(beta), zPos + cos(alpha)*cos(beta)), glm::vec3(0, 1, 0));
        myShader.set_uniform_value("viewMatrix", ME_UNIFORM_TYPE_MAT4X4, &viewMat[0][0]);
        plPosition[0] = xPos;
        plPosition[1] = yPos;
        plPosition[2] = zPos;
        myShader.set_uniform_value("cam_cords", ME_UNIFORM_TYPE_VEC3, plPosition);

        centerPointFigure.move(xPos + sin(alpha), yPos - sin(beta), zPos + cos(alpha)*cos(beta));
        centerPointFigure.draw();

        if(glfwGetMouseButton(testWindow.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            float pos[3] = {sin(alpha), -sin(beta), cos(alpha)*cos(beta)};
            myShader.set_uniform_value( "lightingVec", ME_UNIFORM_TYPE_VEC3, &pos );
        }
        // -------

        // 8. Text output
        myText.clear_string();
        myText.add_to_string("Version 1.0.0\n");
        myText.add_to_string("UPDATE TIME: " + std::to_string( curUT ) + "\n");
        myText.add_to_string("FPS: " + std::to_string( fps ) + "\n");
        myText.add_to_string("x: " + std::to_string(xPos) + "\n");
        myText.add_to_string("y: " + std::to_string(yPos) + "\n");
        myText.add_to_string("z: " + std::to_string(zPos) + "\n");
        myText.add_to_string("alpha: " + std::to_string(alpha) + "\n");
        myText.add_to_string("beta: " + std::to_string(beta) + "\n");
        myText.draw();
        // -------


        // 9. SB PE
        /* Swap front and back buffers */
        glfwSwapBuffers(testWindow.window);

        /* Poll for and process events */
        glfwPollEvents();
        // -------
    }

    glfwTerminate();
    log_file.close();
    return 0;
}
