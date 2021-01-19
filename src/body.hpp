#pragma once
#include "utils.hpp"

class Body {

private:
    inline static unsigned int id_counter = 0;
    std::string m_name;
    vec3 m_position, m_velocity, m_force;
    vec3 m_accelaration, m_jerk;
    real m_radius, m_mass;
    unsigned int m_id;
    std::vector<real> m_history;

public:

    Body() :
    m_name("body"),
    m_position(0.0), m_velocity(0.0),  m_force(0.0),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(1.0), m_mass(1.0),
    m_id(id_counter++)
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y); };
    {};
    // { std::cout << "Body constructor called" << std::endl; };
    Body(std::string name) :
    m_name(name),
    m_position(0.0), m_velocity(0.0), m_force(0.0),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(1.0), m_mass(1.0),
    m_id(id_counter++)
    // { std::cout << "Body constructor called" << std::endl; };
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y);};
    {};
    Body(std::string name, vec3 position, vec3 velocity,
         real radius, real mass) :
    m_name(name),
    m_position(position), m_velocity(velocity), m_force(0.0),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(radius), m_mass(mass),
    m_id(id_counter++)
    // { m_history.push_back(m_position.x); m_history.push_back(m_position.y);
      // m_history.push_back(m_position.x); m_history.push_back(m_position.y);};
    {};

    // IDEA constant force on body (constant force vs gravitational/other)
    // Body(std::string name, vec3 position, vec3 velocity, vec3 force,
    //      real radius, real mass) :
    //      m_name(name),
    //      m_position(position), m_velocity(velocity),  m_force(force),
    //      m_radius(radius), m_mass(mass),
    //      m_id(id_counter++)
    //      {};

    // ~Body() { std::cout << m_name << " Body destructor called" << std::endl; };
    ~Body() {};


    void Update();
    void UpdateNew(real dt);
    // void Translate(vec3 translation);
    void ApplyForce(vec3 f);
    void ApplyParams(vec3 accelaration, vec3 jerk, real mass);


    unsigned int ID() const { return m_id; }
    std::string Name() const { return std::to_string(m_id) + " " + m_name; };

    std::vector<real> GetHistory() { return m_history; };

    vec3 GetPosition() const { return m_position; };
    void SetPosition(vec3 position) { m_position = position; };
    vec3* PositionPtr() { return &m_position; };

// TODO rename to set get
    real GetRadius() const { return m_radius; };
    void SetRadius(real radius);
    real* RadiusPtr() { return &m_radius; };

    real GetMass() const { return m_mass; };
    void SetMass(real mass);
    real* MassPtr() { return &m_mass; }

    vec3 GetVelocity() const { return m_velocity; };
    void SetVelocity(vec3 velocity) { m_velocity = velocity; };
    real GetVelocityMagnitude() const { return glm::length(m_velocity); };
    vec3* VelocityPtr() { return &m_velocity; };

    void PrintVelocity() { std::cout << m_name << ": " <<  glm::to_string(m_velocity) << std::endl; };
    void PrintPosition() { std::cout << m_name << ": " << glm::to_string(m_position) << std::endl; };


    friend std::ostream& operator<<(std::ostream& os, const Body& body);
    friend std::istream& operator>>(std::istream& is, Body& body);
};
