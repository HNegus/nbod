#pragma once
#include "utils.hpp"

class Body {

private:
    inline static unsigned int id_counter = 0;
    std::string m_name;
    vec3 m_position, m_velocity;
    vec3 m_accelaration, m_jerk;
    real m_radius, m_mass;
    Color m_color;
    unsigned int m_id;
    vec3 m_position_old, m_velocity_old;
    vec3 m_accelaration_old, m_jerk_old;
    std::vector<real> m_history;

public:

    Body() :
    m_name("body"),
    m_position(0.0), m_velocity(0.0),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(1.0), m_mass(1.0), m_color({0.0f, 1.0f, 0.0f, 1.0f}),
    m_id(id_counter++),
    m_position_old(0.0), m_velocity_old(0.0),
    m_accelaration_old(0.0), m_jerk_old(0.0)
    {};

    Body(std::string name) :
    m_name(name),
    m_position(0.0), m_velocity(0.0),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(1.0), m_mass(1.0), m_color({0.0f, 1.0f, 0.0f, 1.0f}),
    m_id(id_counter++),
    m_position_old(0.0), m_velocity_old(0.0),
    m_accelaration_old(0.0), m_jerk_old(0.0)
    {};

    Body(std::string name, vec3 position, vec3 velocity,
         real radius, real mass) :
    m_name(name),
    m_position(position), m_velocity(velocity),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(radius), m_mass(mass), m_color({0.0f, 1.0f, 0.0f, 1.0f}),
    m_id(id_counter++),
    m_position_old(0.0), m_velocity_old(0.0),
    m_accelaration_old(0.0), m_jerk_old(0.0)
    {};

    Body(std::string name, vec3 position, vec3 velocity,
         real radius, real mass, Color color) :
    m_name(name),
    m_position(position), m_velocity(velocity),
    m_accelaration(0.0), m_jerk(0.0),
    m_radius(radius), m_mass(mass), m_color(color),
    m_id(id_counter++),
    m_position_old(0.0), m_velocity_old(0.0),
    m_accelaration_old(0.0), m_jerk_old(0.0)
    {};

    // IDEA constant force on body (constant force vs gravitational/other)
    ~Body() {};


    void Evolve(vec3 accelaration, vec3 jerk, real mass);
    void Correct(real dt);
    void Update(real dt);
    void Reset();
    void SaveLocation();


    void SetID(unsigned int id) { m_id = id; };
    unsigned int GetID() const { return m_id; };
    std::string GetIDName() const { return std::to_string(m_id) + " " + m_name; };
    std::string GetName() const { return m_name; };
    void SetName(char *name) { m_name = std::string(name); }

    std::vector<real> GetHistory() { return m_history; };
    void ClearHistory() { m_history.clear(); };

    vec3 GetPosition() const { return m_position; };
    void SetPosition(vec3 position) { m_position = position; };
    vec3* PositionPtr() { return &m_position; };

    real GetRadius() const { return m_radius; };
    void SetRadius(real radius) { m_radius = radius; };
    real* RadiusPtr() { return &m_radius; };

    real GetMass() const { return m_mass; };
    void SetMass(real mass) { m_mass = mass; };
    real* MassPtr() { return &m_mass; }

    vec3 GetVelocity() const { return m_velocity; };
    void SetVelocity(vec3 velocity) { m_velocity = velocity; };
    real GetVelocityMagnitude() const { return glm::length(m_velocity); };
    vec3* VelocityPtr() { return &m_velocity; };

    Color GetColor() const { return m_color; };
    Color* ColorPtr() { return &m_color; };
    void SetColor(Color color) { m_color = color; };

    std::string LogString();
    void PrintVelocity() { std::cout << m_name << ": " <<  glm::to_string(m_velocity) << std::endl; };
    void PrintPosition() { std::cout << m_name << ": " << glm::to_string(m_position) << std::endl; };


    static void SetIDCounter(int val) { id_counter = (unsigned int) val; };
    static unsigned int GetIDCounter() { return id_counter; };
    static void IncIDCounter() { id_counter++; }
    static void DecIDCounter() { id_counter--; };

    friend std::ostream& operator<<(std::ostream& os, const Body& body);
    friend std::istream& operator>>(std::istream& is, Body& body);
};
