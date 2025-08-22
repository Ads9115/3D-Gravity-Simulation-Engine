#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

class Object {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float radius; 
    glm::vec4 color;
    bool glow;

    
    Object(glm::vec3 pos, glm::vec3 vel, float m, float density, glm::vec4 col, bool g, float visualScale);
};
#endif
