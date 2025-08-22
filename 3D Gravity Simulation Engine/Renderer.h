#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include <vector>

class Renderer {
public:
    Renderer(Shader& shader);
    ~Renderer();
    void DrawScene(const std::vector<Object>& objects, Camera& camera);

private:
    Shader& shader;
    unsigned int sphereVAO, vbo, ebo;
    unsigned int indexCount;
    unsigned int gridVAO, gridVBO;
    size_t gridVertexCount;

    void setupSphere();
    void setupGrid(float size, int divisions, float y_level);
    void DrawGrid();
};
#endif
