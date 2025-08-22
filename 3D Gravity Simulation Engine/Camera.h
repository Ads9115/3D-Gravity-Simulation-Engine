#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    float Yaw;
    float Pitch;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(GLFWwindow* window, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
};
#endif
