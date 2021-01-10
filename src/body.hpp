#pragma once
#include "utils.hpp"

class Body {
private:
    std::string m_id;
    float m_xpos, m_ypos, m_radius, m_mass, m_vx, m_vy, m_fx, m_fy;

public:

    Body(std::string id, float xpos, float ypos, float radius,
         float mass, float vx, float vy) :
         m_id(id),
         m_xpos(xpos), m_ypos(ypos),
         m_radius(radius), m_mass(mass),
         m_vx(vx), m_vy(vy), m_fx(0), m_fy(0) {};
    ~Body() {};


    void Move(float x,  float y);

    void Update();
    void ApplyForce(float fx, float fy);
    void Position(float x, float y);
    void Resize(float radius);

    std::string id() { return m_id; };
    float X() const { return m_xpos; };
    float Y() const { return m_ypos; };
    float Radius() const { return m_radius; };
    float Mass() const {return m_mass; }
    void PrintVelocity() { std::cout << m_id << " v x: " << m_vx << " y: " << m_vy << std::endl; };
    void PrintPosition() { std::cout << m_id << " pos x: " << m_xpos << " y: " << m_ypos << std::endl; };
    

};
