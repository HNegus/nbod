#pragma once
#include "utils.hpp"
#include "body.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexbufferlayout.hpp"


class World {


private:
    // TODO set G in this class
    // real G = 6.674 * pow(10, -11);
    RingBuffer<real> m_energy_buffer;
    std::vector<Body*> m_bodies;

    std::vector<real> m_HistoryPositionVbData;
    std::vector<unsigned int> m_HistoryPositionIbData;

    unsigned int m_body_count;

    World(const World&);

    // TODO remove/move to private
    // real m_dt = 60 * 60 * 24 * 365 * 1000;
    // real m_dt = 100000;

    // real m_dt = 60;
    // real m_dt = 1;
    real m_current_time = 0.0;
    real m_dt = 60*30;

public:

    bool m_variable_dt = true;

    World();
    ~World();

    void Clear();

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
    void StoreBody(Body *body);
    void RemoveBody(unsigned int id);

    void Init();
    void ResetBodies();
    void EvolveBodies();
    void CorrectBodies();
    void UpdateBodies();
    void SaveBodyLocations();
    void Step();

    real PotentialEnergy();
    real KineticEnergy();

    void SetBodiesVb(VertexBuffer& vb);
    void SetBodiesIb(IndexBuffer& ib);
    void SetBodiesHistoryPositionsVb(VertexBuffer& vb);
    void SetBodiesHistoryColorsVb(VertexBuffer& vb);
    void SetBodiesHistoryIb(IndexBuffer& ib);


    real GetTime() const { return m_current_time; };
    real GetDeltaTime() const { return m_dt; };
    void SetDeltaTime(real dt) { m_dt = dt; };

    std::vector<Body*> GetBodies() const { return m_bodies; };
    unsigned int GetCount() const { return m_body_count; };

    real TotalEnergy() { return PotentialEnergy() + KineticEnergy(); }
    std::vector<real> TotalEnergyHistory() const { return m_energy_buffer.GetAll(); };

    friend std::ostream& operator<<(std::ostream& os, const World& world);
    friend std::istream& operator>>(std::istream& is, World& world);

};
