#pragma once

#include "utils.hpp"
#include "body.hpp"
#include "camera.hpp"

class SimData {

private:

    Camera *m_camera;


public:

    Body NewBody;
    char NewBodyName[64] = "";
    std::vector<Body*> bodies;
    // TODO for each body, save configuration
    real radius, mass;
    // real max_radius, radius_step, max_mass, mass_step;
    real max_radius, max_mass, delta_radius, delta_mass, delta_position, delta_velocity;

    vec3 camera_position;

    bool show_gui = true;
    bool run_simulation = false;
    bool auto_resize_camera = false;
    bool auto_resize_bodies = true;
    bool track_body = false;
    bool show_history = true;

    int track_body_idx = 0;

    real time_current = 0;

    SimData();
    ~SimData() {};
    void Clear();


    void RegisterBody(Body *body);
    void RegisterBodies(std::vector<Body*> bodies);
    void DeregisterBody(unsigned int id);
    void DeregisterBody(Body *body);
    void RegisterCamera(Camera *camera);
    void Update();

    // static bool ComboGetter(void *data, int idx, const char** out_str) {
    //     std::vector<std::string> names;
    //     for (Body *body: bodies) {
    //         names.push_back(body->Name());
    //     }
    //     *out_str = ((const char*)names[idx].c_str());
    //     return true;
    // }
};
