#include "world.hpp"

World::World() : m_body_count(0) {

}

World::~World() {
    std::cout << "World destructor called" << std::endl;
    for (Body *body: m_bodies) {
        delete body;
    }
}

World::World(const World& old_world) {

    for (Body *body : old_world.Bodies()) {
        std::cout << "tick" << std::endl;
        AddBody(*body);
    }
}

void World::Clear() {
    for (Body *body: m_bodies) {
        delete body;
    }
    m_bodies.clear();
}


void World::Do() {
    for (Body *body : m_bodies) {
        std::cout << body->ID() << " " << body->Name() << std::endl;
    }
    std::cout << std::endl;
}

void World::Step() {
    glm::vec3 pos1, pos2, diff;
    glm::vec3 force;
    double r, F, fx, fy, theta;

    // TODO this
    for (Body *b1: m_bodies) {
        force = glm::vec3(0.0f);
        pos1 = b1->GetPosition();

        for (Body *b2: m_bodies) {

            if (b1->ID() == b2->ID()) continue;

            pos2 = b2->GetPosition();
            diff = pos2 - pos1;

            r = length(diff);
            F = G * (b2->GetMass() / pow(r, 2)) * b1->GetMass();

            theta = atan2(diff.y, diff.x);
            fx = F * cos(theta);
            fy = F * sin(theta);

            force.x += fx;
            force.y += fy;
            b1->ApplyForce(force);
        }
    }

    UpdateBodies();
}

void World::StoreBody(Body *body)
{
    m_bodies.push_back(body);
    m_body_count++;
    return;
}


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
                     glm::vec3 position, glm::vec3 velocity,
                     float radius, float mass)
{
    Body *body = new Body(name, position, velocity, radius, mass);
    StoreBody(body);
    return body;
}



void World::UpdateBodies() {
    for (Body *body: m_bodies) {
        body->Update();
    }
}


double World::KineticEnergy() {
    double total = 0.0;
    for (Body *body: m_bodies) {
        total += body->GetMass() * pow(body->GetVelocityMagnitude(), 2) * 0.5;
    }
    return total;
}

double World::PotentialEnergy() {
    double total = 0.0;
    float r;
    for (Body *b1: m_bodies) {
        for (Body *b2: m_bodies) {
            if (b1->ID() == b2->ID()) continue;
            r = length(b2->GetPosition() - b1->GetPosition());
            total += - G * (b1->GetMass() / r) * b2->GetMass();

        }
    }
    return total / 2;
}



void World::SetBodiesVb(VertexBuffer& vb) {
    std::vector<float> bodies_data;

    glm::vec3 pos(0.0f);
    float x, y, r;

    // TODO clean
    for (Body *body : m_bodies) {
        pos = body->GetPosition();
        x = pos.x;
        y = pos.y;
        r = body->GetRadius();

        bodies_data.insert(end(bodies_data), {x - r, y - r, r, x, y});
        bodies_data.insert(end(bodies_data), {x + r, y - r, r, x, y});
        bodies_data.insert(end(bodies_data), {x - r, y + r, r, x, y});
        bodies_data.insert(end(bodies_data), {x + r, y + r, r, x, y});

    }

    vb.Update(bodies_data.data(), sizeof (float) * bodies_data.size());

}

void World::SetBodiesIb(IndexBuffer& ib) {
    std::vector<unsigned int> bodies_data;

    for (unsigned int i = 0; i < m_body_count; i++) {

        bodies_data.insert(end(bodies_data), {0 + i * 4, 1 + i * 4, 2 + i * 4,
                                1 + i * 4, 3 + i * 4, 2 + i * 4});
    }

    // TODO magic numbers
    ib.Update(bodies_data.data(), bodies_data.size());
}


void World::SetBodiesHistoryPositionsVb(VertexBuffer& vb) {
    std::vector<float> history;
    std::vector<float> history_data;


    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }

        for (float v: history) {
            history_data.push_back(v);
        }
    }

    vb.Update(history_data.data(), sizeof (float) * history_data.size());

}


void World::SetBodiesHistoryColorsVb(VertexBuffer& vb) {
    std::vector<float> history;
    std::vector<unsigned char> history_data;

    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }


        for (size_t i = 0; i < history.size(); i += 2) {
            history_data.insert(end(history_data), {255, 0, 0, 100});
        }

    }

    vb.Update(history_data.data(), sizeof (unsigned char) * history_data.size());
}

void World::SetBodiesHistoryIb(IndexBuffer& ib) {

    std::vector<float> history;
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
        // std::cout << body;
        is.ignore(5);
    }
    return is;
}
