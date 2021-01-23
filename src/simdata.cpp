#include "simdata.hpp"

SimData::SimData() : bodies(), max_radius(), max_mass(), delta_radius(),
    delta_mass(), delta_position(100.0f), delta_velocity(100.0f),
    camera_position(0.0f)
{
    delta_radius = 0.001 * RADIUS_MOON;
    delta_mass = 0.001 * MASS_MOON;
    max_radius = FLT_MAX;
    max_mass = FLT_MAX;
}

void SimData::RegisterBody(Body* body) {

    bodies.push_back(body);
    Update();
    initialize_world = true;
}

void SimData::RegisterBodies(std::vector<Body*> in_bodies) {
    for (Body *body: in_bodies) {
        bodies.push_back(body);
    }
    Update();
}

void SimData::RegisterCamera(Camera *camera) {
    m_camera = camera;
    camera_position = m_camera->GetPosition();
}

void SimData::Clear() {
    bodies.clear();
    Update();
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
