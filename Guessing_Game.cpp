#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Guesser.h"
#include "Shader.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
//#define _CRT_SECURE_NO_WARNINGS
//#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

std::vector<float> vertexVector; //Vertices
bool held;
float x, y;
mat dataset(784, 1);
Guesser prediction;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    x = (2.0f * xpos) / width - 1.0f;
    y = 1.0f - (2.0f * ypos) / height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        held = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        held = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        vertexVector.clear();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        unsigned char* pixels = new unsigned char[560 * 560];
        glReadPixels(0, 0, 560, 560, GL_RED, GL_UNSIGNED_BYTE, pixels);
        stbi_flip_vertically_on_write(true);

        unsigned char* resized_pixels = new unsigned char[28 * 28];
        stbir_resize_uint8(pixels, 560, 560, 0, resized_pixels, 28, 28, 0, 1);
        stbi_write_png("screenshot.png", 28, 28, 1, resized_pixels, 28); //Used for testing

        for (int i = 27; i >= 0; i--) {
            for (int j = 0; j < 28; j++) {
                dataset((i * 28 + j), 0) = resized_pixels[i * 28 + j];
            }
            std::cout << std::endl;
        }

        prediction.predict(dataset);
        delete[] pixels;
        delete[] resized_pixels;
    }
    
}



int main()
{

    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(560, 560, "Guesser", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 560, 560);

    Shader shader("shader.vert", "shader.frag");

    // Create VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexVector.size(), vertexVector.data(), GL_STATIC_DRAW);

    // Create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.UseShader();

    double currentTime = glfwGetTime(); // Frame limiter
    double lastTime = currentTime;

    while (!glfwWindowShouldClose(window)) {

        currentTime = glfwGetTime();

        if (currentTime - lastTime >= 1.0 / 60) // 60 fps
        {
            lastTime = currentTime;

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (held) {
              
                float vertices[] = {
               x - 0.075f,  y + 0.075f, 0.0f,
               x - 0.075f, y - 0.075f, 0.0f,
                x + 0.075f, y - 0.075f, 0.0f,

                x + 0.075f,  y + 0.075f, 0.0f,
               x - 0.075f,  y + 0.075f, 0.0f,
                x + 0.075f, y - 0.075f, 0.0f
                };

                for (int i = 0; i < 18; i++) {

                    vertexVector.push_back(vertices[i]);

                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexVector.size(), vertexVector.data(), GL_DYNAMIC_DRAW);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertexVector.size() / 3);
            glBindVertexArray(0);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }

        
    }

    for (int i = 27; i >= 0; i--) {
        for (int j = 0; j < 28; j++) {
            if (dataset((i * 28 + j), 0) != 0) {
                std::cout << "1";
            }
            else {
                std::cout << "0";
            }
        }
        std::cout << std::endl;
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader.DeleteShader();

    glfwTerminate();

    return 0;
}