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

void World::StepNew() {

    Body *b1, *b2;

    vec3 diff_pos(0), diff_v(0), dacc(0), djerk(0);
    real r2, v2, rv_r2, r, r3;

    for (size_t i = 0; i < m_bodies.size(); i++) {
        b1 = m_bodies[i];
        for (size_t j = i + 1; j < m_bodies.size(); j++) {
            b2 = m_bodies[j];
            // if (b1->ID() == b2->ID()) continue;

            diff_pos = b2->GetPosition() - b1->GetPosition();
            diff_v = b2->GetVelocity() - b1->GetVelocity();

            r2 = glm::dot(diff_pos, diff_pos);
            v2 = glm::dot(diff_v, diff_v);
            rv_r2 = glm::dot(diff_pos, diff_v) / r2;
            r = sqrt(r2);
            r3 = r * r2;

            dacc = diff_pos / r3;
            djerk = (diff_v - 3.0 * rv_r2 * diff_pos) / r3;
            std::cout << diff_pos.x << " " << diff_pos.y << std::endl;
            std::cout << dacc.x << " " << dacc.y << std::endl;
            // std::cout << r << " " << r2 << " " << r3 << std::endl;
            std::cout << std::endl;

            b1->ApplyParams(dacc, djerk, b2->GetMass());
            b2->ApplyParams(-dacc, -djerk, b1->GetMass());
        }
    }

    UpdateBodiesNew();

}

void World::Step() {
    vec3 pos1, pos2, diff;
    vec3 force;
    double r, F, fx, fy, theta;

    // TODO this
    for (Body *b1: m_bodies) {
        force = vec3(0.0f);
        pos1 = b1->GetPosition();

        for (Body *b2: m_bodies) {

            if (b1->ID() == b2->ID()) continue;

            pos2 = b2->GetPosition();
            diff = pos2 - pos1;

            r = length(diff);
            // F = G * (b2->GetMass() / pow(r, 3));

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


void World::UpdateBodies() {
    for (Body *body: m_bodies) {
        body->Update();
    }
}


void World::UpdateBodiesNew() {
    for (Body *body: m_bodies) {
        body->UpdateNew(m_dt);
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
    real r;
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
                                              c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f});
        bodies_data.insert(end(bodies_data), {x + r, y - r, r, x, y,
                                              c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f});
        bodies_data.insert(end(bodies_data), {x - r, y + r, r, x, y,
                                              c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f});
        bodies_data.insert(end(bodies_data), {x + r, y + r, r, x, y,
                                              c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f});

    }

    vb.Update(bodies_data.data(), sizeof (real) * bodies_data.size());

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
            history_data.insert(end(history_data), {c.r, c.g, c.b, c.a});
        }

    }

    vb.Update(history_data.data(), sizeof (unsigned char) * history_data.size());
}

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
        std::cout << body;
        is.ignore(5);
    }
    return is;
}
