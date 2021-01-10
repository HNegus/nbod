#include "body.hpp"

void Body::ApplyForce(float fx, float fy) {
    // std::cout << id() << " fx: " << fx << " fy: " << fy << std::endl;
    m_fx += fx;
    m_fy += fy;
}

void Body::Update() {

    m_vx += m_fx / m_mass * DELTA_TIME;
    m_vy += m_fy / m_mass * DELTA_TIME;

    // std::cout << m_xpos;
    m_xpos += m_vx * DELTA_TIME;
    m_ypos += m_vy * DELTA_TIME;
    // std::cout << id() << " dx: " << m_vx * DELTA_TIME << " dy: " << m_vy * DELTA_TIME << std::endl;

    m_fx = 0;
    m_fy = 0;
}

void Body::Move(float x,  float y) {
    m_xpos += x;
    m_ypos += y;
}
