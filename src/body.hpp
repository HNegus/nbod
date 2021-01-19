#pragma once
#include "utils.hpp"

class Body {

private:
    inline static unsigned int id_counter = 0;
    std::string m_name;
    glm::vec3 m_position, m_velocity, m_force;
    glm::vec3 m_accelaration, m_jerk;
    float m_radius, m_mass;
    unsigned int m_id;
    std::vector<float> m_history;

public:

    Body() :
    m_name("body"),
    m_position(0.0f), m_velocity(0.0f),  m_force(0.0f),
    m_accelaration(0.0f), m_jerk(0.0f),
    m_radius(1.0f), m_mass(1.0f),
    m_id(id_counter++)
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y); };
    {};
    // { std::cout << "Body constructor called" << std::endl; };
    Body(std::string name) :
    m_name(name),
    m_position(0.0f), m_velocity(0.0f), m_force(0.0f),
    m_accelaration(0.0f), m_jerk(0.0f),
    m_radius(1.0f), m_mass(1.0f),
    m_id(id_counter++)
    // { std::cout << "Body constructor called" << std::endl; };
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y);};
    {};
    Body(std::string name, glm::vec3 position, glm::vec3 velocity,
         float radius, float mass) :
    m_name(name),
    m_position(position), m_velocity(velocity), m_force(0.0f),
    m_accelaration(0.0f), m_jerk(0.0f),
    m_radius(radius), m_mass(mass),
    m_id(id_counter++)
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y);};
    {};

    // IDEA constant force on body (constant force vs gravitational/other)
    // Body(std::string name, glm::vec3 position, glm::vec3 velocity, glm::vec3 force,
    //      float radius, float mass) :
    //      m_name(name),
    //      m_position(position), m_velocity(velocity),  m_force(force),
    //      m_radius(radius), m_mass(mass),
    //      m_id(id_counter++)
    //      {};

    // ~Body() { std::cout << m_name << " Body destructor called" << std::endl; };
    ~Body() {};


    void Update();
    // void Translate(glm::vec3 translation);
    void ApplyForce(glm::vec3 f);

    unsigned int ID() const { return m_id; }
    std::string Name() const { return std::to_string(m_id) + " " + m_name; };

    std::vector<float> GetHistory() { return m_history; };

    glm::vec3 GetPosition() const { return m_position; };
    void SetPosition(glm::vec3 position) { m_position = position; };
    glm::vec3* PositionPtr() { return &m_position; };

// TODO rename to set get
    float GetRadius() const { return m_radius; };
    void SetRadius(float radius);
    float* RadiusPtr() { return &m_radius; };

    float GetMass() const { return m_mass; };
    void SetMass(float mass);
    float* MassPtr() { return &m_mass; }

    glm::vec3 GetVelocity() const { return m_velocity; };
    void SetVelocity(glm::vec3 velocity) { m_velocity = velocity; };
    float GetVelocityMagnitude() const { return glm::length(m_velocity); };
    glm::vec3* VelocityPtr() { return &m_velocity; };

    void PrintVelocity() { std::cout << m_name << ": " <<  glm::to_string(m_velocity) << std::endl; };
    void PrintPosition() { std::cout << m_name << ": " << glm::to_string(m_position) << std::endl; };


    friend std::ostream& operator<<(std::ostream& os, const Body& body);
    friend std::istream& operator>>(std::istream& is, Body& body);
};
