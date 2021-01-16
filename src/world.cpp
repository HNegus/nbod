#include "world.hpp"

World::World() : m_body_count(0) {

}

World::~World() {
    std::cout << "World destructor called" << std::endl;
    for (Body *body : m_bodies) {
        delete body;
    }
}

World::World(const World& old_world) {

    for (Body *body : old_world.Bodies()) {
        std::cout << "tick" << std::endl;
        AddBody(*body);
    }
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

        // history_data.push_back(history[0]);
        // history_data.push_back(history[1]);
        for (float v: history) {
            history_data.push_back(v);
        }
        // for (size_t i; i < history.size(); i += 2) {
        //     history_data.push_back(history[i]);
        //     history_data.push_back(history[i]);
        //
        // }

        // history_data.pop_back();
        // history_data.pop_back();

        // history_data.push_back(0);
        // history_data.push_back(0);

        // history_data.push_back(history[1]);

        // history_data.push_back(history.end()[-1]);
        // history_data.push_back(history.end()[-2]);

        // history_data.push_back(0);
        // history_data.push_back(0);
        // history_data.push_back(0);

        // history_data.push_back(65535);


    }

    // if (!history_data.empty())
        // history_data.pop_back();

    // std::cout << "pos: ";
    // for (auto v: history_data) {
        // if (v == 0xFF) {
            // std::cout << v << " ";
        // }
    // }
    // std::cout << std::endl;

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

        // history_data.insert(end(history_data), {0, 0, 0, 0});

        for (size_t i = 0; i < history.size(); i += 2) {
            history_data.insert(end(history_data), {255, 0, 0, 100});
        }
        // history_data.pop_back();

        // history_data.insert(end(history_data), {0, 0, 0, 0});
        // history_data.insert(end(history_data), {0xFF, 0xFF, 0xFF, 0xFF});
        // history_data.insert(end(history_data), {0xFF, 0xFF, 0xFF, 0xFF});
        // history_data.insert(end(history_data), {0xFF, 0xFF, 0xFF, 0xFF});


        // history_data.insert(end(history_data), {0, 0, 0, 0});
        // history_data.push_back(65535);

        // history_data.insert(end(history_data), {0, 0, 0, 0});

    }

    // if (!history_data.empty())
        // history_data.pop_back();

    // std::cout << "col: ";
    // for (auto v: history_data) {
    //     // if (v == RESTART_INDEX) {
    //         std::cout << (int) v << " ";
    //     // }
    // }
    // std::cout << std::endl;

    vb.Update(history_data.data(), sizeof (unsigned char) * history_data.size());
}

void World::SetBodiesHistoryIb(IndexBuffer& ib) {

    std::vector<float> history;
    std::vector<unsigned int> history_data;
    unsigned int i = 0;
    // history_data.push_back(0);

    for (Body *body: m_bodies) {
        history = body->GetHistory();
        if (history.size() < 4) {
            continue;
        }

        // history_data.push_back(i++);
        for (size_t j = 0; j < history.size(); j += 2) {
            history_data.push_back(i++);
        }
        // i++;
        // history_data.push_back(i-1);
        // history_data.push_back(i);

        history_data.push_back(RESTART_INDEX);

        // history_data.push_back(0);


        // history_data.push_back(i++);
    }

    // if (!history_data.empty())
        // history_data.pop_back();

    // std::cout << "ib: ";
    // for (auto v: history_data) {
    //     // if (v == RESTART_INDEX) {
    //         std::cout << v << " ";
    //     // }
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;



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
        std::cout << body;
        is.ignore(5);
    }
    return is;
}
