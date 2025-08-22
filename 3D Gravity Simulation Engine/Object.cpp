#include "Object.h"
#include <cmath>
#include <glm/gtc/constants.hpp> 

const float sizeRatio = 3000000.0f;

Object::Object(glm::vec3 pos, glm::vec3 vel, float m, float density, glm::vec4 col, bool g, float visualScale) {
    position = pos;
    velocity = vel;
    mass = m;
    color = col;
    glow = g;

    
    float physicalRadius = pow(((3.0f * mass / density) / (4.0f * glm::pi<float>())), (1.0f / 3.0f));

   
    radius = (physicalRadius / sizeRatio) * visualScale;
}
