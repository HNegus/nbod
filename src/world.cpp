#include "world.hpp"

World::World() : m_index(0) {
    // m_layout.Push<float>(2);
    // m_layout.Push<float>(1);
    // m_layout.Push<float>(2);
}

World::~World() {
    for (Body *body : m_bodies) {
        delete body;
    }
}

void World::Do() {
    for (Body *body : m_bodies) {
        body->Move(DISTANCE_MOON_EARTH / 100, 0);
        std::cout << body->X();
    }
    Update();
}

void World::Step() {
    glm::vec2 pos1, pos2;
    glm::vec2 force(0.0f, 0.0f);
    double r, F, fx, fy, theta;

    std::ofstream file;
    file.open ("output.txt");

    for (int i = 0; i < 1; i++) {

        for (Body *b1: m_bodies) {
            pos1.x = b1->X();
            pos1.y = b1->Y();

            for (Body *b2: m_bodies) {
                if (b1 == b2) continue;
                pos2.x = b2->X();
                pos2.y = b2->Y();


                r = sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));


                F = G * (b2->Mass() / pow(r, 2)) * b1->Mass();
                // std::cout << b1->id() << "r: " << r << std::endl;


                theta = atan2(pos2.y - pos1.y, pos2.x - pos1.x);
                fx = F * cos(theta);
                fy = F * sin(theta);


                // exit(0);
                force.x += fx;
                force.y += fy;
                b1->ApplyForce(fx, fy);

            }
            if (b1->id() == "moon") {
                file << fx << std::endl;
            }
            // b1->ApplyForce(force.x, force.y);
            // std::cout << b1->id() << "y: " << pos1.x << std::endl;
            // b1->PrintVelocity();
            // b1->PrintPosition();
            // std::cout << (MASS_EARTH - MASS_MOON) << std::endl;

            // std::cout << std::endl;

        }

    }

    file.close();

    for (Body *body: m_bodies) {
        body->Update();
    }
    Update();
}


void World::AddBody(std::string id, float x, float y, float radius,
                   float mass, float vx, float vy) {
    Body *body = new Body(id, x, y, radius, mass, vx, vy);
    m_bodies.push_back(body);
    m_index++;
    Update();
}

void World::Update() {
    SetVertices();
    SetIndices();
}


void World::Bodies() {
    for (Body *body : m_bodies) {
        std::cout << body->X() << std::endl;
    }
}

// void World::Renew() {
//     SetVertices();
//     SetIndices();
//
//     m_vb = VertexBuffer(m_data.data(), m_index * 4 * 5 * sizeof (float));
//     m_ib = IndexBuffer(m_indices.data(), m_index * 6);
//     m_va.AddBuffer(m_vb, m_layout);
//     m_va.Bind();
//     m_ib.Bind();
// }

double World::KineticEnergy() {
    double total = 0.0;
    for (Body *b: m_bodies) {
        total += b->Mass() * pow(b->Velocity(), 2) * 0.5;
    }
    return total;
}

double World::PotentialEnergy() {
    double total = 0.0;
    float r;
    for (Body *b1: m_bodies) {
        for (Body *b2: m_bodies) {
            if (b1 == b2) continue;
            r = sqrt(pow(b2->X() - b1->X(), 2) + pow(b2->Y() - b1->Y(), 2));
            total += - G * (b1->Mass() / r) * b2->Mass();
            // total += b->Mass() * pow(b->Velocity(), 2) * 0.5;

        }
    }
    return total / 2;
}

void World::SetVertices() {
    std::vector<float> data;
    float x, y, r;
    for (Body *body : m_bodies) {
        x = body->X();
        y = body->Y();
        r = body->Radius();

        data.insert(end(data), {x - r, y - r, r, x, y});
        data.insert(end(data), {x + r, y - r, r, x, y});
        data.insert(end(data), {x - r, y + r, r, x, y});
        data.insert(end(data), {x + r, y + r, r, x, y});
    }
    // for (auto d : data) {
    //     std::cout << d << " ";
    // }
    // std::cout << std::endl;
    // TODO: magic numbers
    m_vbdata = data;
    m_vbsize = sizeof (float) * m_index * 4 * 5;

    // VertexBuffer vb(m_data.data(), sizeof (float) * m_index * 4 * 5);

    // return vb;
}


void World::SetIndices() {
    std::vector<unsigned int> data;
    for (unsigned int i = 0; i < m_index; i++) {

        data.insert(end(data), {0 + i * 4, 1 + i * 4, 2 + i * 4,
                                1 + i * 4, 3 + i * 4, 2 + i * 4});
    }
    // for (auto d : data) {
    //     std::cout << d << " ";
    // }
    // std::cout << std::endl;
    // TODO magic numbers
    m_ibdata = data;
    m_ibsize = m_index * 6;

    // IndexBuffer ib(data.data(), m_index * 6);

    // return ib;
}
