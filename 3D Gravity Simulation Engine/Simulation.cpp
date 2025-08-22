#include "Simulation.h"

const double G = 6.6743e-11;

Simulation::Simulation() {
    pause = false;
}

void Simulation::Update() {
    if (pause) return;

    for (size_t i = 0; i < objects.size(); ++i) {
        glm::vec3 totalForce(0.0f);
        for (size_t j = 0; j < objects.size(); ++j) {
            if (i == j) continue;

            glm::vec3 diff = objects[j].position - objects[i].position;
            float distance = glm::length(diff);

            if (distance > 0) {
                float simDistance = distance * 1000.0f;
                double Gforce = (G * objects[i].mass * objects[j].mass) / (simDistance * simDistance);
                glm::vec3 direction = glm::normalize(diff);
                totalForce += direction * (float)(Gforce / objects[i].mass);
            }
        }
        objects[i].velocity += totalForce / 96.0f;
    }

    for (auto& obj : objects) {
        obj.position += obj.velocity / 94.0f;
    }
}

void Simulation::AddObject(const Object& obj) {
    objects.push_back(obj);
}
