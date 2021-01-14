#pragma once
#include "utils.hpp"
#include "body.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexbufferlayout.hpp"


class World {


private:
    std::vector<Body*> m_bodies;
    std::vector<float> m_vbdata;
    std::vector<unsigned int> m_ibdata;
    // VertexArray m_va;
    // VertexBuffer m_vb;
    // IndexBuffer m_ib;
    // VertexBufferLayout m_layout;

    unsigned int m_body_count;
    unsigned int m_vbsize;
    unsigned int m_ibsize;

    // TODO implement copy constructor
    World(const World&);

public:
    // std::vector<Body> m_bodies_prime;


    World();
    ~World();


    void StoreBody(Body *body);
    Body* AddBody();
    Body* AddBody(Body other_body);
    Body* AddBody(std::string name);
    Body* AddBody(std::string name,
                  glm::vec3 position, glm::vec3 velocity,
                  float radius, float mass);
    std::vector<Body*> Bodies() const { return m_bodies; };
    void RemoveBody(unsigned int id);
    unsigned int GetCount() const { return m_body_count; };
    void Do();

    // void Renew();
    void UpdateWorld();
    void UpdateBodies();
    void Step();
    void SetVertices();
    void SetIndices();

    // VertexArray va() { return m_va; }
    float* vbdata() { return m_vbdata.data(); }
    unsigned int* ibdata() { return m_ibdata.data(); }
    unsigned int vbsize() { return m_vbsize; }
    unsigned int ibsize() { return m_ibsize; }
    double PotentialEnergy();
    double KineticEnergy();
    double TotalEnergy() { return PotentialEnergy() + KineticEnergy(); }


    // IndexBuffer ib() { return m_ib; }
    // VertexBufferLayout layout() { return m_layout; }

    friend std::ostream& operator<<(std::ostream& os, const World& world);
    friend std::istream& operator>>(std::istream& is, World& world);


    // std::vector<float> Data() { return m_data; };


};
