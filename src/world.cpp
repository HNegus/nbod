#include "world.hpp"

World::World() : m_body_count(0) {
    // m_layout.Push<float>(2);
    // m_layout.Push<float>(1);
    // m_layout.Push<float>(2);
    // std::cout << "World constructor called" << std::endl;

}

World::~World() {
    // std::cout << "World destructor called" << std::endl;
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
        // body->Id();
        std::cout << body->ID() << " " << body->Name() << std::endl;
    }
    std::cout << std::endl;
    UpdateWorld();
}

void World::Step() {
    glm::vec3 pos1, pos2, diff;
    glm::vec3 force;
    double r, F, fx, fy, theta;

    // std::ofstream file;
    // file.open ("output.txt");

    // TODO this
    for (int i = 0; i < 1; i++) {


        for (Body *b1: m_bodies) {
            force = glm::vec3(0.0f);
            pos1 = b1->GetPosition();

            for (Body *b2: m_bodies) {
                if (b1->ID() == b2->ID()) continue;
                pos2 = b2->GetPosition();
                diff = pos2 - pos1;


                r = length(diff);


                F = G * (b2->GetMass() / pow(r, 2)) * b1->GetMass();
                // std::cout << b1->id() << "r: " << r << std::endl;


                theta = atan2(diff.y, diff.x);
                fx = F * cos(theta);
                fy = F * sin(theta);


                // exit(0);
                force.x += fx;
                force.y += fy;
                b1->ApplyForce(force);

            }
            // if (b1->id() == "moon") {
                // file << fx << std::endl;
            // }
            // b1->ApplyForce(force.x, force.y);
            // std::cout << b1->id() << "y: " << pos1.x << std::endl;
            // b1->PrintVelocity();
            // b1->PrintPosition();
            // std::cout << (MASS_EARTH - MASS_MOON) << std::endl;

            // std::cout << std::endl;

        }

    }

    // file.close();
    UpdateBodies();
    UpdateWorld();
}

void World::StoreBody(Body *body)
{
    m_bodies.push_back(body);
    m_body_count++;
    UpdateWorld();
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


void World::UpdateWorld() {
    SetVertices();
    SetIndices();
}

void World::UpdateBodies() {
    for (Body *body: m_bodies) {
        body->Update();
    }
}


// void World::Renew() {
//     SetVertices();
//     SetIndices();
//
//     m_vb = VertexBuffer(m_data.data(), m_body_count * 4 * 5 * sizeof (float));
//     m_ib = IndexBuffer(m_indices.data(), m_body_count * 6);
//     m_va.AddBuffer(m_vb, m_layout);
//     m_va.Bind();
//     m_ib.Bind();
// }

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
            // total += b.GetMass() * pow(b.Velocity(), 2) * 0.5;

        }
    }
    return total / 2;
}

void World::SetVertices() {
    std::vector<float> data;
    glm::vec3 pos(0.0f);
    float x, y, r;
    // TODO clean
    for (Body *body : m_bodies) {
        pos = body->GetPosition();
        x = pos.x;
        y = pos.y;
        r = body->GetRadius();

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
    m_vbsize = sizeof (float) * m_body_count * 4 * 5;

    // VertexBuffer vb(m_data.data(), sizeof (float) * m_body_count * 4 * 5);

    // return vb;
}


void World::SetIndices() {
    std::vector<unsigned int> data;
    for (unsigned int i = 0; i < m_body_count; i++) {

        data.insert(end(data), {0 + i * 4, 1 + i * 4, 2 + i * 4,
                                1 + i * 4, 3 + i * 4, 2 + i * 4});
    }
    // for (auto d : data) {
    //     std::cout << d << " ";
    // }
    // std::cout << std::endl;
    // TODO magic numbers
    m_ibdata = data;
    m_ibsize = m_body_count * 6;

    // IndexBuffer ib(data.data(), m_body_count * 6);

    // return ib;
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
