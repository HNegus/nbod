#pragma once

#include "utils.hpp"
#include "body.hpp"
#include "camera.hpp"

class SimData {

private:

    Camera *m_camera;


public:

    Body new_body;
    char new_body_name[64] = "";
    char scene_name[64] = "";
    std::vector<Body*> bodies;

    // TODO for each body, save configuration
    real radius, mass;
    real delta_radius, delta_mass, delta_position, delta_velocity;
    vec3 camera_position;

    bool headless = false;
    bool logging = false;
    bool limit_framerate = true;
    bool variable_dt = true;
    bool initialize_world = true;
    bool show_gui = true;
    bool run_simulation = false;
    bool auto_resize_camera = false;
    bool auto_resize_bodies = true;
    bool track_body = false;
    bool show_history = true;

    unsigned int history_resolution = 10;
    int track_body_idx = 0;
    real time_current = 0;
    real dt = 1000.0;

    SimData();
    ~SimData() {};
    void Clear();


    void RegisterBody(Body *body);
    void RegisterBodies(std::vector<Body*> bodies);
    void DeregisterBody(unsigned int id);
    void RegisterCamera(Camera *camera);
    void Update();
};
