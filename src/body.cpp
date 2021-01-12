#include "body.hpp"


void Body::ApplyForce(glm::vec3 f) {
    // std::cout << id() << " fx: " << fx << " fy: " << fy << std::endl;
    m_force += f;
}

void Body::Update() {

    m_velocity += m_force / m_mass * DELTA_TIME;
    // m_vx += m_fx / m_mass * DELTA_TIME;
    // m_vy += m_fy / m_mass * DELTA_TIME;

    // std::cout << m_xpos;
    m_position += m_velocity * DELTA_TIME;
    // m_xpos += m_vx * DELTA_TIME;
    // m_ypos += m_vy * DELTA_TIME;
    // std::cout << id() << " dx: " << m_vx * DELTA_TIME << " dy: " << m_vy * DELTA_TIME << std::endl;

    m_force = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Body::Move(glm::vec3 translation) {
    m_position += translation;
    // m_xpos += x;
    // m_ypos += y;
}
