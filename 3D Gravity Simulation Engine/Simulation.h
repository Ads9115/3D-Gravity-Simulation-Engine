#ifndef SIMULATION_H
#define SIMULATION_H

#include "Object.h"
#include <vector>

class Simulation {
public:
    std::vector<Object> objects;
    bool pause;

    Simulation();
    void Update();
    void AddObject(const Object& obj);
};
#endif
