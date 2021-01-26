#pragma once

#include "utils.hpp"
#include "world.hpp"
#include "camera.hpp"
#include "simdata.hpp"

class Scene {

private:

    std::string m_name;
    World& m_world;
    Camera& m_camera;
    SimData& m_config;
    std::string m_dir;

public:
    Scene();
    // Scene(std::string name, World& world)
    Scene(std::string name, World& world, Camera& camera, SimData& config) :
          m_name(name), m_world(world), m_camera(camera), m_config(config)
    {};
    ~Scene() {};

    void Load();
    void LoadScene();
    void LoadSave();
    void LoadWorld();
    void LoadCamera();
    void LoadConfig();

    void Save();
    void SaveWorld();
    void SaveCamera();
    void SaveConfig();
};
