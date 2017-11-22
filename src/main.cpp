#include <glwrapper.h>
#include "glfw3.h"
#include <thread>
#include <iostream>
#include "ShadersLoader.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Can not create glfw window! \n";
        glfwTerminate();
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing glew. Exiting.\n";
        return -1;
    }
#endif

    auto vertexSource = shaders::loadShaderSourceFromFile("resources/shaders/vertex.glsl").c_str();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetShaderInfoLog(vertexShader, sizeof(info), NULL, info);
        std::cerr << "ERROR::SHADER LINK_FAILED\n" << info;
        return 0;
    }

    auto fragmentSource = shaders::loadShaderSourceFromFile("resources/shaders/fragment.glsl").c_str();

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetShaderInfoLog(fragmentShader, sizeof(info), NULL, info);
        std::cerr << "ERROR::SHADER LINK_FAILED\n" << info;
        return 0;
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetProgramInfoLog(program, sizeof(info), NULL, info);
        std::cerr << "ERROR::PROGRAM LINK_FAILED\n" << info;
        std::cerr << vertexSource << std::endl;
        std::cerr << fragmentSource << std::endl;

        glDeleteProgram(program);
        glfwTerminate();
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat coor[] = {
            -0.2f, 0.4f,
            0.0f, -0.4f,
            0.2f, 0.4f,

            -0.05f, -0.2f,
            -0.1f, -0.2f,
            -0.1f, -0.6f,

            -0.1f, -0.6f,
            -0.05f, -0.6f,
            -0.05f, -0.2f,

            0.05f, -0.2f,
            0.1f, -0.2f,
            0.1f, -0.6f,

            0.1f, -0.6f,
            0.05f, -0.6f,
            0.05f, -0.2f,

            -0.2f, 0.4f,
            -0.25f,0.4f,
            -0.25f, -0.1f,

            0.2f, 0.4f,
            0.25f, -0.1f,
            0.25f, 0.4f,

            0.0f, 0.5f,
            -0.0398f, 0.4293f,
            0.0f, 0.4f,

            0.0f, 0.5f,
            -0.0563f, 0.5f,
            -0.0398f, 0.4293f,

            0.0f, 0.5f,
            -0.0398f, 0.5707f,
            -0.0563f, 0.5f,

            0.0f, 0.5f,
            0.0f, 0.6f,
            -0.0398f, 0.5707f,

            0.0f, 0.5f,
            0.0398f, 0.5707f,
            0.0f, 0.6f,

            0.0f, 0.5f,
            0.0563f, 0.5f,
            0.0398f, 0.5707f,

            0.0f, 0.5f,
            0.0398f, 0.4293f,
            0.0563f, 0.5f,

            0.0f, 0.5f,
            0.0f, 0.4f,
            0.0398f, 0.4293f
    };

    GLuint indexes[] = {
            0, 1, 2,
            4, 5, 3,
            7, 8, 6,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35,
            36, 37, 38,
            39, 40, 41,
            42, 43, 44

    };

    GLuint buffer;
    glGenBuffers(1, &buffer);

    GLuint elements;
    glGenBuffers(1, &elements);

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 90, coor, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 45, indexes, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.4f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, 45, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::nanoseconds(15000));
    }

    glfwTerminate();
    return 0;
}