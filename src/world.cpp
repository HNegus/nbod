#include "world.hpp"

World::World() : m_energy_buffer(1000), m_body_count(0) {}

World::~World() {
    Clear();
}

/* Copy constructor. */
World::World(const World& old_world) : m_energy_buffer(1000), m_body_count(0)
{

    for (Body *body : old_world.GetBodies()) {
        AddBody(*body);
    }
}

/* Remove all bodies from world. */
void World::Clear() {
    for (Body *body: m_bodies) {
        delete body;
    }
    m_bodies.clear();
}

/* Push new body to list of bodies. */
void World::StoreBody(Body *body)
{
    m_bodies.push_back(body);
    m_body_count++;
    return;
}

/* Add body to world. */
Body* World::AddBody()
{
    Body *body = new Body;
    StoreBody(body);
    return body;
}

Body* World::AddBody(Body other_body)
{
    Body *body = new Body(other_body);
    StoreBody(body);
    return body;
}

Body* World::AddBody(std::string name)
{
    Body *body = new Body(name);
    StoreBody(body);
    return body;
}

Body* World::AddBody(std::string name,
                     vec3 position, vec3 velocity,
                     real radius, real mass)
{
    Body *body = new Body(name, position, velocity, radius, mass);
    StoreBody(body);
    return body;
}


Body* World::AddBody(std::string name,
                     vec3 position, vec3 velocity,
                     real radius, real mass, Color color)
{
    Body *body = new Body(name, position, velocity, radius, mass, color);
    StoreBody(body);
    return body;
}


/* Evolve bodies once, before calculating new position, accelation etc.*/
void World::Init() {
    EvolveBodies();
}

/* Reset all bodies. */
void World::ResetBodies() {
    for (Body *body: m_bodies) {
        body->Reset();
    }
}

/* Compute body interactions and evolve them. */
void World::EvolveBodies() {

    Body *b1, *b2;

    vec3 diff_pos(0), diff_v(0), dacc(0), djerk(0);
    real r2, v2, rv_r2, r, r3;

    for (size_t i = 0; i < m_bodies.size(); i++) {
        b1 = m_bodies[i];
        for (size_t j = i + 1; j < m_bodies.size(); j++) {
            b2 = m_bodies[j];

            diff_pos = b2->GetPosition() - b1->GetPosition();
            diff_v = b2->GetVelocity() - b1->GetVelocity();

            r2 = glm::dot(diff_pos, diff_pos);
            v2 = glm::dot(diff_v, diff_v);
            rv_r2 = glm::dot(diff_pos, diff_v) / r2;
            r = sqrt(r2);
            r3 = r * r2;

            dacc = diff_pos / r3;
            djerk = (diff_v - 3.0 * rv_r2 * diff_pos) / r3;

            b1->Evolve(dacc, djerk, b2->GetMass());
            b2->Evolve(-dacc, -djerk, b1->GetMass());
        }
    }
}

/* Correct all bodies. */
void World::CorrectBodies() {
    for (Body *body: m_bodies) {
        body->Correct(m_dt);
    }
}

/* Advace bodies to new positions. */
void World::UpdateBodies() {
    ResetBodies();

    for (Body *body: m_bodies) {
        body->Update(m_dt);
    }
}


/* Advance world to next timestep. */
void World::Step() {

    UpdateBodies();
    EvolveBodies();
    CorrectBodies();
    m_energy_buffer.Add(PotentialEnergy() + KineticEnergy());

}

/* Return total kinetic energy in world. */
real World::KineticEnergy() {
    real total = 0.0;
    for (Body *body: m_bodies) {
        total += body->GetMass() * pow(body->GetVelocityMagnitude(), 2) * 0.5;
    }

    return total;
}

/* Return total potential energy in world. */
real World::PotentialEnergy() {
    real total = 0.0;
    real r;
    for (Body *b1: m_bodies) {
        for (Body *b2: m_bodies) {
            if (b1->GetID() == b2->GetID()) continue;
            r = length(b2->GetPosition() - b1->GetPosition());
            total += - G * (b1->GetMass() / r) * b2->GetMass();

        }
    }

    return total / 2;
}


/************************* OpenGL buffer management. *************************/
/* Set body vertexbuffer. */
void World::SetBodiesVb(VertexBuffer& vb) {
    std::vector<float> bodies_data;

    vec3 pos(0.0f);
    float x, y, r;

    // TODO clean
    for (Body *body : m_bodies) {
        pos = body->GetPosition();
        x = (float) pos.x;
        y = (float) pos.y;
        r = (float) body->GetRadius();

        Color c = body->GetColor();

        bodies_data.insert(end(bodies_data), {x - r, y - r, r, x, y,
                                              c.r, c.g, c.b, c.a});
        bodies_data.insert(end(bodies_data), {x + r, y - r, r, x, y,
                                              c.r, c.g, c.b, c.a});
        bodies_data.insert(end(bodies_data), {x - r, y + r, r, x, y,
                                              c.r, c.g, c.b, c.a});
        bodies_data.insert(end(bodies_data), {x + r, y + r, r, x, y,
                                              c.r, c.g, c.b, c.a});
    }

    vb.Update(bodies_data.data(), sizeof (real) * bodies_data.size());
}

/* Set body indexbuffer. */
void World::SetBodiesIb(IndexBuffer& ib) {
    std::vector<unsigned int> bodies_data;

    for (unsigned int i = 0; i < m_body_count; i++) {

        bodies_data.insert(end(bodies_data), {0 + i * 4, 1 + i * 4, 2 + i * 4,
                                1 + i * 4, 3 + i * 4, 2 + i * 4});
    }

    // TODO magic numbers
    ib.Update(bodies_data.data(), bodies_data.size());
}

/* Set history position vertexbuffer. */
void World::SetBodiesHistoryPositionsVb(VertexBuffer& vb) {
    std::vector<real> history;
    std::vector<float> history_data;


    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }

        for (real v: history) {
            history_data.push_back((float) v);
        }
    }

    vb.Update(history_data.data(), sizeof (float) * history_data.size());
}

/* Set history color vertexbuffer. */
void World::SetBodiesHistoryColorsVb(VertexBuffer& vb) {
    std::vector<real> history;
    std::vector<unsigned char> history_data;

    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }


        for (size_t i = 0; i < history.size(); i += 2) {
            Color c = body->GetColor();
            history_data.insert(end(history_data), {(unsigned char) (c.r * 255),
                                                    (unsigned char) (c.g * 255),
                                                    (unsigned char) (c.b * 255),
                                                    (unsigned char) (c.a * 255)});
        }

    }

    vb.Update(history_data.data(), sizeof (unsigned char) * history_data.size());
}

/* Set history indexbuffer. */
void World::SetBodiesHistoryIb(IndexBuffer& ib) {

    std::vector<real> history;
    std::vector<unsigned int> history_data;
    unsigned int i = 0;

    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }

        for (size_t j = 0; j < history.size(); j += 2) {
            history_data.push_back(i++);
        }

        history_data.push_back(RESTART_INDEX);
    }

    ib.Update(history_data.data(), history_data.size());
}


std::ostream& operator<<(std::ostream& os, const World& world) {
    for (Body *body: world.m_bodies) {
        os << (*body);
        os << "---" << std::endl;
    }

    return os;
}

std::istream& operator>>(std::istream& is, World& world) {

    Body body;
    while (!is.eof()) {
        is >> body;
        world.AddBody(body);
        is.ignore(5);
    }

    return is;
}
