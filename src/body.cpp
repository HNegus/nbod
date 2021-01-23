#include "body.hpp"

void Body::SetRadius(real radius) {
    m_radius = radius;
}

// void Body::ApplyForce(vec3 f) {
//     // std::cout << id() << " fx: " << fx << " fy: " << fy << std::endl;
//     m_force += f;
// }

// void Body::Update() {
//
//     m_velocity += m_force / m_mass * DELTA_TIME;
//     // m_vx += m_fx / m_mass * DELTA_TIME;
//     // m_vy += m_fy / m_mass * DELTA_TIME;
//
//     // std::cout << m_xpos;
//     m_position += m_velocity * DELTA_TIME;
//     // m_xpos += m_vx * DELTA_TIME;
//     // m_ypos += m_vy * DELTA_TIME;
//     // std::cout << id() << " dx: " << m_vx * DELTA_TIME << " dy: " << m_vy * DELTA_TIME << std::endl;
//
//     m_force = vec3(0.0, 0.0, 0.0);
//
//     m_history.push_back(m_position.x);
//     m_history.push_back(m_position.y);
// }

// void Body::Translate(vec3 translation) {
    // m_position += translation;
// }

void Body::Evolve(vec3 accelaration, vec3 jerk, real mass) {
    m_accelaration += G * mass * accelaration;
    m_jerk += G * mass * jerk;
}
void Body::Update(real dt) {

    // vec3 np = m_velocity * dt + m_accelaration * dt * dt/2.0 + m_jerk * dt * dt * dt/6.0;
    // std::cout << "new pos: " << np.x << " " << np.y << std::endl;
    m_position += m_velocity * dt + m_accelaration * dt * dt/2.0 + m_jerk * dt * dt * dt/6.0;
    m_velocity += m_accelaration * dt + m_jerk * dt * dt/2.0;

    // m_accelaration = vec3(0);
    // m_jerk = vec3(0);

    m_history.push_back(m_position.x);
    m_history.push_back(m_position.y);
}

void Body::Reset() {
    m_position_old = m_position;
    m_velocity_old = m_velocity;
    m_accelaration_old = m_accelaration;
    m_jerk_old = m_jerk;

    m_accelaration = vec3(0);
    m_jerk = vec3(0);
}

void Body::Correct(real dt) {
    m_velocity = m_velocity_old + (m_accelaration_old + m_accelaration) * dt/2.0
                                + (m_jerk_old - m_jerk) * dt * dt/12.0;
    m_position = m_position_old + (m_velocity_old + m_velocity)*dt/2.0
                                + (m_accelaration_old - m_accelaration)*dt*dt/12.0;
}

std::ostream& operator<<(std::ostream& os, const Body& body) {

    os << body.m_id << std::endl;
    os << body.m_name << std::endl;
    os << body.m_position.x << " " << body.m_position.y << std::endl;
    os << body.m_velocity.x << " " << body.m_velocity.y << std::endl;
    os << body.m_force.x << " " << body.m_force.y << std::endl;
    os << body.m_radius << std::endl;
    os << body.m_mass << std::endl;
    os << (int) (body.m_color.r * 255.0f) << " " << (int) (body.m_color.g * 255.0f) << " " << (int) (body.m_color.b * 255.0f) << " " << (int) (body.m_color.a * 255.0f) << std::endl;

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
    int c[4];
    is >> c[0] >> c[1] >> c[2] >> c[3];
    body.m_color = Color(c[0], c[1], c[2], c[3]);

    // std::cout << "Loaded: " << body.m_name << std::endl;
    return is;
}
