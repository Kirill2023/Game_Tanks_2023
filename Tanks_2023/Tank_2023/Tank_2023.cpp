
// Std. Includes
#include <string>
#include <vector>
#include <list>
#include<Windows.h>


#include <iostream>
#include <algorithm>
#include <cmath>

#include <cstdint>
#include <typeinfo>
#include <memory>
#include <thread>
#include <mutex>
#include <corecrt_math_defines.h>
#include "Prototypes.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Other Libs
#include "Camera.h"
#include "OGL.h"
#include <ppl.h>


// Properties
GLuint screenWidth = 1600, screenHeight = 900;

const int Num = 10;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void Do_Movement();

// Camera
Camera camera(glm::vec3(3.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 600, lastY = 400;
bool firstMouse = true;
bool MOUSE_BUTTON_LEFT = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



using namespace std;
using namespace tank;



int main()
{
    setlocale(LC_ALL, "Russian");

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "curves", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);


    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);


    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    OGL::Init(screenWidth, screenHeight);

    glm::mat4 model(1);
    glm::mat4 view(1);
    glm::mat4 projection(1);


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // 1. Добовляем виды прототипов в реестр прототипов

    Prototypes::Init();



    // 2. Создаем обыект field

    Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::field));
    Barrier::_vpObject.back()->set_Ref(vec3(0, 0, -0.5));

    Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::Wall_H));
    Barrier::_vpObject.back()->set_Ref(vec3(0, 10, -0.5));

    Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::Wall_H));
    Barrier::_vpObject.back()->set_Ref(vec3(0, -10, -0.5));

    Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::Wall_V));
    Barrier::_vpObject.back()->set_Ref(vec3(10, 0, -0.5));

    Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::Wall_V));
    Barrier::_vpObject.back()->set_Ref(vec3(-10, 0, -0.5));



    for (int x = 0; x < 18; ++x)
        for (int y = 0; y < 18; ++y)
        {
            switch (MAP[x][y])
            {
            case 1:
                Barrier::_vpObject.push_back(Object::createObject(Type_ID::Barrier, SubType_ID::brick));
                Barrier::_vpObject.back()->set_Ref(vec3(x - 9, y - 9, 0));
                break;

            case 2:
                Tank::_vpObject.push_back(Object::createObject(Type_ID::Tank, SubType_ID::middle));
                Tank::_vpObject.back()->set_Ref(vec3(x - 9, y - 9, -0.2));
                break;
            }
        }






    // 1. Создаем контейнеры обыектов






    int64_t countr = 0;

    while (!glfwWindowShouldClose(window))
    {

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);


        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.7f, 0.8f, 0.9f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);






        Barrier::Tick(countr);
        Tank::Tick(countr);
        Bullet::Tick(countr);

        Bullet::Draw(view, projection);
        Tank::Draw(view, projection);
        Barrier::Draw(view, projection);

        Object::Collision_test(Barrier::_vpObject, Bullet::_vpObject);
        Object::Collision_test(Barrier::_vpObject, Tank::_vpObject);
        Object::Collision_test(Bullet::_vpObject, Tank::_vpObject);
        Object::Collision_test(Tank::_vpObject, Tank::_vpObject);
        Object::Collision_test(Bullet::_vpObject, Bullet::_vpObject);

        Barrier::Clearing();
        Tank::Clearing();
        Bullet::Clearing();



        // Swap the buffers
        glfwSwapBuffers(window);
        Sleep(10);
        countr++;
        if (countr > 60000)
        {
            std::cout << "Время вышло. Выход из программы";
            break;
        }
    }


    glfwTerminate();
    return 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%







// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if ((keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(FORWARD, deltaTime * 4);
    if ((keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(BACKWARD, deltaTime * 4);
    if ((keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(LEFT, deltaTime * 4);
    if ((keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) && keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(RIGHT, deltaTime * 4);

       
      


}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            MOUSE_BUTTON_LEFT = true;
        else if (action == GLFW_RELEASE)
            MOUSE_BUTTON_LEFT = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (MOUSE_BUTTON_LEFT)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else
        firstMouse = true;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}




