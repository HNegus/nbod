#include "simdata.hpp"

SimData::SimData() : bodies(), max_radius(), max_mass(), delta_radius(),
    delta_mass(), delta_position(100.0f), delta_velocity(100.0f),
    camera_position(0.0f)
{
    delta_radius = 0.001 * RADIUS_MOON;
    delta_mass = 0.001 * MASS_MOON;
    max_radius = 100*RADIUS_EARTH;
    max_mass = 10*MASS_EARTH;
}

void SimData::RegisterBody(Body* body) {

    bodies.push_back(body);
    Update();
}

void SimData::RegisterCamera(Camera *camera) {
    m_camera = camera;
    camera_position = m_camera->GetPosition();
}

// TODO reset bounds
void SimData::Update() {

    // float radius, mass;
    // glm::vec3 position, velocity;

    // for (Body *body: bodies) {
        // radius = body->Radius();
        // mass = body->Mass();

    // }

    camera_position = m_camera->GetPosition();
}
