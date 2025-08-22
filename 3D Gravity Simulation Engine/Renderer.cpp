#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Shader& shader) : shader(shader) {
    setupSphere();
    
    setupGrid(60000.0f, 50, -4000.0f);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
}

void Renderer::DrawScene(const std::vector<Object>& objects, Camera& camera) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    
    DrawGrid();

    
    for (const auto& obj : objects) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);
        model = glm::scale(model, glm::vec3(obj.radius));

        shader.setMat4("model", model);
        shader.setVec4("objectColor", obj.color);
        shader.setInt("GLOW", obj.glow);
        shader.setInt("isGrid", 0); 

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::DrawGrid() {
    shader.use();
    glm::mat4 model = glm::mat4(1.0f); 
    shader.setMat4("model", model);
    shader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    shader.setInt("isGrid", 1); 
    shader.setInt("GLOW", 0);

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridVertexCount);
    glBindVertexArray(0);
}

void Renderer::setupGrid(float size, int divisions, float y_level) {
    std::vector<float> vertices;
    float step = size / divisions;
    float halfSize = size / 2.0f;

    for (int i = 0; i <= divisions; ++i) {
        
        vertices.push_back(-halfSize + i * step); vertices.push_back(y_level); vertices.push_back(-halfSize);
        vertices.push_back(-halfSize + i * step); vertices.push_back(y_level); vertices.push_back(halfSize);

       
        vertices.push_back(-halfSize); vertices.push_back(y_level); vertices.push_back(-halfSize + i * step);
        vertices.push_back(halfSize);  vertices.push_back(y_level); vertices.push_back(-halfSize + i * step);
    }
    gridVertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


void Renderer::setupSphere() {
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 32;
    const unsigned int Y_SEGMENTS = 32;
    const float PI = 3.14159265359f;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            positions.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        if (!oddRow) {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else {
            for (int x = X_SEGMENTS; x >= 0; --x) {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = indices.size();

    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
}
