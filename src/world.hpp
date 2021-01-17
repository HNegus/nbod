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

    // std::vector<float> m_BodiesVbData;
    // std::vector<unsigned int> m_BodiesIbData;
    // unsigned int m_BodiesVbSize;
    // unsigned int m_BodiesIbSize;


    std::vector<float> m_HistoryPositionVbData;
    std::vector<unsigned int> m_HistoryPositionIbData;
    unsigned int m_HistoryPositionVbSize;
    unsigned int m_HistoryPositionIbSize;

    // VertexArray m_va;
    // VertexBuffer m_vb;
    // IndexBuffer m_ib;
    // VertexBufferLayout m_layout;

    unsigned int m_body_count;




    // TODO implement copy constructor
    World(const World&);

public:
    // std::vector<Body> m_bodies_prime;


    World();
    ~World();


    void StoreBody(Body *body);
    // TODO Give body parameters that are similar to other bodies.
    Body* AddBody();
    Body* AddBody(Body other_body);
    Body* AddBody(std::string name);
    Body* AddBody(std::string name,
                  glm::vec3 position, glm::vec3 velocity,
                  float radius, float mass);
    std::vector<Body*> Bodies() const { return m_bodies; };
    // TODO
    void RemoveBody(unsigned int id);
    void Clear();
    unsigned int GetCount() const { return m_body_count; };
    void Do();

    void UpdateBodies();
    void Step();


    void SetBodiesVb(VertexBuffer& vb);
    void SetBodiesIb(IndexBuffer& ib);
    void SetBodiesHistoryPositionsVb(VertexBuffer& vb);
    void SetBodiesHistoryColorsVb(VertexBuffer& vb);
    void SetBodiesHistoryIb(IndexBuffer& ib);

    double PotentialEnergy();
    double KineticEnergy();
    double TotalEnergy() { return PotentialEnergy() + KineticEnergy(); }


    friend std::ostream& operator<<(std::ostream& os, const World& world);
    friend std::istream& operator>>(std::istream& is, World& world);


    // std::vector<float> Data() { return m_data; };


};
