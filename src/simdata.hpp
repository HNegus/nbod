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
    float max_radius, max_mass, delta_mass, delta_radius, delta_position, delta_velocity;

    glm::vec3 camera_position;

    SimData();
    ~SimData() {};

    void RegisterBody(Body *body);
    void DeregisterBody(unsigned int id);
    void DeregisterBody(Body *body);
    void RegisterCamera(Camera *camera);
    void Update();
};
