#include "body.hpp"


/* Add to jerk and acceleration. */
void Body::Evolve(vec3 acceleration, vec3 jerk, real mass) {
    m_acceleration += G * mass * acceleration;
    m_jerk += G * mass * jerk;
}

/* Get more accurate estimate for velocity and position by using taylor series. */
void Body::Correct(real dt) {
    m_velocity = m_velocity_old + (m_acceleration_old + m_acceleration) * dt/2.0
                                + (m_jerk_old - m_jerk) * dt * dt/12.0;
    m_position = m_position_old + (m_velocity_old + m_velocity)*dt/2.0
                                + (m_acceleration_old - m_acceleration)*dt*dt/12.0;
}

/* Calculate new position and velocity, based on acceleration and jerk. */
void Body::Update(real dt) {

    m_position += m_velocity * dt + m_acceleration * dt * dt/2.0 + m_jerk * dt * dt * dt/6.0;
    m_velocity += m_acceleration * dt + m_jerk * dt * dt/2.0;

}


/* Save current position to history; */
void Body::SaveLocation() {
    m_history.push_back(m_position.x);
    m_history.push_back(m_position.y);
}

/* Save current parameters and reset acceleration and jerk. */
void Body::Reset() {
    m_position_old = m_position;
    m_velocity_old = m_velocity;
    m_acceleration_old = m_acceleration;
    m_jerk_old = m_jerk;

    m_acceleration = vec3(0);
    m_jerk = vec3(0);
}

std::string Body::LogString() {
    std::string result;

    result += std::to_string(m_position.x) + " " + std::to_string(m_position.y) + " " + std::to_string(m_position.z);
    result += " ";
    result += std::to_string(m_velocity.x) + " " + std::to_string(m_velocity.y) + " " + std::to_string(m_velocity.z);
    return result;
}

std::ostream& operator<<(std::ostream& os, const Body& body) {

    os << body.m_id << std::endl;
    os << body.m_name << std::endl;
    os << body.m_position.x << " " << body.m_position.y << " " << body.m_position.z << std::endl;
    os << body.m_velocity.x << " " << body.m_velocity.y << " " << body.m_velocity.z << std::endl;
    os << body.m_radius << std::endl;
    os << body.m_mass << std::endl;
    os << (int) (body.m_color.r * 255.0f) << " " << (int) (body.m_color.g * 255.0f) << " " << (int) (body.m_color.b * 255.0f) << " " << (int) (body.m_color.a * 255.0f) << std::endl;

    return os;
}


std::istream& operator>>(std::istream& is, Body& body) {
    is >> body.m_id;
    is >> body.m_name;
    is >> body.m_position.x >> body.m_position.y >> body.m_position.z;
    is >> body.m_velocity.x >> body.m_velocity.y >> body.m_velocity.z;
    is >> body.m_radius;
    is >> body.m_mass;
    int c[4];
    is >> c[0] >> c[1] >> c[2] >> c[3];
    body.m_color = Color(c[0], c[1], c[2], c[3]);

    return is;
}
