#pragma once
#include "utils.hpp"

class Body {
private:
    inline static unsigned int id_counter = 0;
    std::string m_name;
    glm::vec3 m_position, m_force, m_velocity;
    float m_radius, m_mass;
    const unsigned int m_id;

public:

    Body(std::string name, float xpos, float ypos, float radius,
         float mass, float vx, float vy) :
         m_name(name), m_position(xpos, ypos, 0.0f), m_force(vx, vy, 0.0f),
         m_velocity(vx, vy, 0.0f),
         m_radius(radius), m_mass(mass),
        m_id(id_counter++) {};
    ~Body() {};


    void Move(glm::vec3 translation);

    void Update();
    void ApplyForce(glm::vec3 f);
    void Resize(float radius);

    unsigned int ID() const { return m_id; }
    std::string Name() const { return m_name; };

    glm::vec3 GetPosition() const { return m_position; };
    glm::vec3 *PositionPtr() { return &m_position; };

    float Radius() const { return m_radius; };
    float* RadiusPtr() { return &m_radius; };

    float Mass() const { return m_mass; }
    float *MassPtr() { return &m_mass; }
    float Velocity() const { return glm::length(m_velocity); };
    void PrintVelocity() { std::cout << m_name << ": " <<  glm::to_string(m_velocity) << std::endl; };
    // TODO print glm::vec
    void PrintPosition() { std::cout << m_name << ": " << glm::to_string(m_position) << std::endl; };


};
