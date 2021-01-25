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

void SimData::DeregisterBody(unsigned int id) {
    size_t index = -1;
    for (size_t i = 0; i < bodies.size(); i++) {
        if (bodies[i]->GetID() == id) {
            index = i;
            break;
        }
    }

    if (index >= 0) {
        bodies.erase(bodies.begin() + index);
    }
}


/* Set camera pointer. */
void SimData::RegisterCamera(Camera *camera) {
    m_camera = camera;
    camera_position = m_camera->GetPosition();
}

/* Reset configuration. */
void SimData::Clear() {
    bodies.clear();

    limit_framerate = true;
    variable_dt = true;
    track_body_idx = 0;
    track_body = false;
    initialize_world = true;
    run_simulation = false;
    time_current = 0;
    dt = 1000.0;

    new_body_name[0] = 0;
    scene_name[0] = 0;
    Update();
}

/* Update configuration data. */
void SimData::Update() {
    camera_position = m_camera->GetPosition();
}
