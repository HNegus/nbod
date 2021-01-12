#pragma once

#include "utils.hpp"

class SimData {

private:

public:

    // TODO for each body, save configurations
    std::vector<Body *> m_bodies;
    float max_radius, radius_step, max_mass, mass_step;
    glm::vec3 min_position, max_position, position_step,
              min_velocity, max_velocity, velocity_step;

    SimData() {};
    ~SimData() {};

    void RegisterBody(Body *body);
    void DeregisterBody(Body *body);
    void Update();
}
