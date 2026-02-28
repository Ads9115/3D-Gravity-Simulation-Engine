#include "Simulation.h"

const double G = 6.6743e-11;
const double METERS_PER_KM = 1000.0;
const double TIME_SCALE = 2400.0;


Simulation::Simulation() {
    pause = false;
}

void Simulation::Update(float deltaTime) {
    if (pause || objects.empty() || deltaTime <= 0.0f) {
        return;
    }

    const double step = static_cast<double>(deltaTime) * TIME_SCALE;
    std::vector<glm::dvec3> accelerations(objects.size(), glm::dvec3(0.0));

    for (size_t i = 0; i < objects.size(); ++i) {
        
        for (size_t j = 0; j < objects.size(); ++j) {
            if (i == j) {
                continue;
            }

            glm::dvec3 diff = glm::dvec3(objects[j].position - objects[i].position);
            double distanceKm = glm::length(diff);
            if (distanceKm <= 0.0) {
                continue;
            }
            double distanceMeters = distanceKm * METERS_PER_KM;
            glm::dvec3 direction = glm::normalize(diff);
            double acceleration = (G * static_cast<double>(objects[j].mass)) / (distanceMeters * distanceMeters);
            accelerations[i] += direction * acceleration;
        }
        
    }

    for (size_t i = 0; i < objects.size(); ++i) {
        glm::dvec3 velocity = glm::dvec3(objects[i].velocity);
        velocity += accelerations[i] * step;
        objects[i].velocity = glm::vec3(velocity);

        glm::dvec3 position = glm::dvec3(objects[i].position);
        position += (velocity * step) / METERS_PER_KM;
        objects[i].position = glm::vec3(position);
    }
}

void Simulation::AddObject(const Object& obj) {
    objects.push_back(obj);
}
