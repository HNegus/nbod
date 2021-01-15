#include "body.hpp"

void Body::SetRadius(float radius) {
    m_radius = radius;
}

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

    m_history.push_back(m_position.x);
    m_history.push_back(m_position.y);
}

// void Body::Translate(glm::vec3 translation) {
    // m_position += translation;
// }


std::ostream& operator<<(std::ostream& os, const Body& body) {

    os << body.m_id << std::endl;
    os << body.m_name << std::endl;
    os << body.m_position.x << " " << body.m_position.y << std::endl;
    os << body.m_velocity.x << " " << body.m_velocity.y << std::endl;
    os << body.m_force.x << " " << body.m_force.y << std::endl;
    os << body.m_radius << std::endl;
    os << body.m_mass << std::endl;

    return os;
}


std::istream& operator>>(std::istream& is, Body& body) {
    is >> body.m_id;
    is >> body.m_name;
    is >> body.m_position.x >> body.m_position.y;
    is >> body.m_velocity.x >> body.m_velocity.y;
    is >> body.m_force.x >> body.m_force.y;
    is >> body.m_radius;
    is >> body.m_mass;
    return is;
}
