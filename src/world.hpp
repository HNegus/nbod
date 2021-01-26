#pragma once
#include "utils.hpp"
#include "body.hpp"
#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexbufferlayout.hpp"


class World {


private:
    // IDEA variable parameter G
    RingBuffer<real> m_energy_buffer;
    std::vector<Body*> m_bodies;

    std::vector<real> m_HistoryPositionVbData;
    std::vector<unsigned int> m_HistoryPositionIbData;

    unsigned int m_body_count;



    real m_current_time = 0.0;
    real m_dt = 60*30;
    unsigned int m_history_resolution = 1;
    unsigned int m_tick = 0;

    World(const World&);

public:

    bool m_variable_dt = true;

    World();
    ~World();

    void Clear();

    // IDEA Give body parameters that are similar to other bodies.
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
    void ClearBodyHistories();
    void Step();

    real PotentialEnergy();
    real KineticEnergy();

    void SetBodiesVb(VertexBuffer& vb);
    void SetBodiesIb(IndexBuffer& ib);
    void SetBodiesHistoryPositionsVb(VertexBuffer& vb);
    void SetBodiesHistoryColorsVb(VertexBuffer& vb);
    void SetBodiesHistoryIb(IndexBuffer& ib);


    std::string GetBodyNames();
    std::string LogString();

    unsigned int GetHistoryResolution() const { return m_history_resolution; }
    void SetHistoryResolution(unsigned int resolution) { m_history_resolution = resolution; }
    real GetTime() const { return m_current_time; };
    real GetDeltaTime() const { return m_dt; };
    void SetDeltaTime(real dt) { m_dt = dt; };

    std::vector<Body*> GetBodies() const { return m_bodies; };
    unsigned int GetBodyCount() const { return m_body_count; };

    real TotalEnergy() { return PotentialEnergy() + KineticEnergy(); }
    std::vector<real> TotalEnergyHistory() const { return m_energy_buffer.GetAll(); };

    friend std::ostream& operator<<(std::ostream& os, const World& world);
    friend std::istream& operator>>(std::istream& is, World& world);

};
