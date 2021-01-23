#pragma once
#include "utils.hpp"
#include "body.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexbufferlayout.hpp"


class World {


private:
    // real G = 6.674 * pow(10, -11);
    RingBuffer<real> m_energy_buffer;
    std::vector<Body*> m_bodies;

    // std::vector<real> m_BodiesVbData;
    // std::vector<unsigned int> m_BodiesIbData;
    // unsigned int m_BodiesVbSize;
    // unsigned int m_BodiesIbSize;


    std::vector<real> m_HistoryPositionVbData;
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

    // real m_dt = 60 * 60 * 24 * 365 * 1000;
    // real m_dt = 100000;
    // real m_dt = 60*30;
    real m_dt = 60;

    World();
    ~World();


    void StoreBody(Body *body);
    // TODO Give body parameters that are similar to other bodies.
    Body* AddBody();
    Body* AddBody(Body other_body);
    Body* AddBody(std::string name);
    Body* AddBody(std::string name,
                  vec3 position, vec3 velocity,
                  real radius, real mass);
    Body* AddBody(std::string name,
                vec3 position, vec3 velocity,
                real radius, real mass, Color color);
    std::vector<Body*> Bodies() const { return m_bodies; };
    // TODO
    void RemoveBody(unsigned int id);
    void Clear();
    unsigned int GetCount() const { return m_body_count; };
    void Do();



    // void UpdateBodiesNew();
    void Init();
    // void Evolve();
    void Step();
    // void StepNew();

    void ResetBodies();
    void UpdateBodies();
    void EvolveBodies();
    void CorrectBodies();


    void SetBodiesVb(VertexBuffer& vb);
    void SetBodiesIb(IndexBuffer& ib);
    void SetBodiesHistoryPositionsVb(VertexBuffer& vb);
    void SetBodiesHistoryColorsVb(VertexBuffer& vb);
    void SetBodiesHistoryIb(IndexBuffer& ib);

    real PotentialEnergy();
    real KineticEnergy();
    real TotalEnergy() { return PotentialEnergy() + KineticEnergy(); }
    std::vector<real> TotalEnergyHistory() const { return m_energy_buffer.GetAll(); };

    friend std::ostream& operator<<(std::ostream& os, const World& world);
    friend std::istream& operator>>(std::istream& is, World& world);


    // std::vector<real> Data() { return m_data; };


};
