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

    unsigned int m_index;
    unsigned int m_vbsize;
    unsigned int m_ibsize;

public:

    World();
    ~World();

    void AddBody(std::string name, float x, float y, float radius, float mass,
                 float vx, float vy);
    std::vector<Body*> Bodies();

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




    // std::vector<float> Data() { return m_data; };


};
