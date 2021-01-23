#include "simdata.hpp"

SimData::SimData() :
    bodies(), delta_radius(100.0),
    delta_mass(100.0), delta_position(100.0), delta_velocity(100.0),
    camera_position(0.0)
{}

/* Add bodies to track. */
void SimData::RegisterBody(Body* body) {

    bodies.push_back(body);
    Update();
    initialize_world = true;
}

/* Add bodies to track. */
void SimData::RegisterBodies(std::vector<Body*> in_bodies) {
    for (Body *body: in_bodies) {
        bodies.push_back(body);
    }
    Update();
}

/* Set camera pointer. */
void SimData::RegisterCamera(Camera *camera) {
    m_camera = camera;
    camera_position = m_camera->GetPosition();
}

/* Reset configuration. */
void SimData::Clear() {
    bodies.clear();
    track_body_idx = 0;
    track_body = false;
    initialize_world = true;
    run_simulation = false;
    time_current = 0;
    Update();
}

/* Update configuration data. */
void SimData::Update() {
    camera_position = m_camera->GetPosition();
}
