#include "simdata.hpp"

SimData::SimData() : bodies(), max_radius(), max_mass(),
    mass_step(), min_position(1.0f), max_position(1.0f),
              min_velocity(-1.0f), max_velocity(0.0f),
              camera_position(0.0f)
{
    max_radius = 100*RADIUS_EARTH;
    max_mass = 100*RADIUS_MOON;
}

void SimData::RegisterBody(Body *body) {

    bodies.push_back(body);
    Update();
}

void SimData::RegisterCamera(Camera *camera) {
    m_camera = camera;
    camera_position = m_camera->GetPosition();
}

// TODO reset bounds
void SimData::Update() {

    float radius, mass;
    glm::vec3 position, velocity;

    for (Body *body: bodies) {
        radius = body->Radius();
        mass = body->Mass();

        position = 10.0f * body->GetPosition();
        velocity = 10.0f * body->GetVelocity();

        // if (10 * radius > max_radius) max_radius = 10 * radius;
        // if (10 * mass > max_mass) max_mass = 10 * mass;


        if (min_position.x > 10 * position.x) min_position.x = 10 * position.x;
        if (min_position.y > 10 * position.y) min_position.y = 10 * position.y;

        if (max_position.x < 10 * position.x) max_position.x = 10 * position.x;
        if (max_position.y < 10 * position.y) max_position.y = 10 * position.y;
    }

    camera_position = m_camera->GetPosition();
}
