#pragma once

#include "utils.hpp"
#include "body.hpp"
#include "camera.hpp"

class SimData {

private:

    Camera *m_camera;


public:

    std::vector<Body*> bodies;
    // TODO for each body, save configuration
    float radius, mass;
    // float max_radius, radius_step, max_mass, mass_step;
    float max_radius, max_mass, mass_step;
    glm::vec3 min_position, max_position,
              min_velocity, max_velocity;

    glm::vec3 camera_position;

    SimData();
    ~SimData() {};

    void RegisterBody(Body *body);
    void DeregisterBody(Body *body);
    void RegisterCamera(Camera *camera);
    void Update();
};
