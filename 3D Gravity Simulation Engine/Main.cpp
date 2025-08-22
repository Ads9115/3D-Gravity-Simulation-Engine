#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Simulation.h"
#include "Renderer.h"

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

Camera camera(glm::vec3(0.0f, 15000.0f, 40000.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gravity Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

   
    const char* vs = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main()\n{\n   gl_Position = projection * view * model * vec4(aPos, 1.0);\n}\0";
    const char* fs = "#version 330 core\nout vec4 FragColor;\nuniform vec4 objectColor;\nuniform bool GLOW;\nuniform bool isGrid;\nvoid main()\n{\n    if(isGrid) FragColor = objectColor;\n    else if(GLOW) FragColor = vec4(objectColor.rgb * 1.5, 1.0);\n    else FragColor = objectColor;\n}\n\0";

    Shader shader(vs, fs);
    Renderer renderer(shader);
    Simulation simulation;

    float sunMass = 1.989e30f;
    simulation.AddObject(Object(glm::vec3(0), glm::vec3(0), sunMass, 1410.0f, glm::vec4(1, 1, 0, 1), true, 10.0f));

    float planetMass1 = 5.972e24f;
    float dist1 = 14960.0f;
    float v1 = sqrt((float)(6.6743e-11 * sunMass) / (dist1 * 1000.0f));
    simulation.AddObject(Object(glm::vec3(dist1, 0, 0), glm::vec3(0, 0, -v1), planetMass1, 5515.0f, glm::vec4(0, 0, 1, 1), false, 100.0f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.ProcessKeyboard(window, deltaTime);
        simulation.Update();

        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150000.0f);
        shader.setMat4("projection", projection);

        renderer.DrawScene(simulation.objects, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }
    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
