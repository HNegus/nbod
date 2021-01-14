#pragma once

#include <filesystem>
#include "utils.hpp"
#include "world.hpp"
#include "camera.hpp"
#include "simdata.hpp"

class Scene {

private:

    const std::string scene_dir = "../scenes/";
    std::string m_name;
    World& m_world;
    Camera& m_camera;
    SimData& m_config;

public:
    Scene();
    // Scene(std::string name) : m_name(name) {};
    Scene(std::string name, World& world, Camera& camera, SimData& config) :
          m_name(name), m_world(world), m_camera(camera), m_config(config)
    { std::cout << "Making scene" << std::endl; };
    ~Scene() {};

    void Load();
    void LoadWorld();
    void LoadCamera();
    void LoadConfig();

    void Save();
    void SaveWorld();
    void SaveCamera();
    void SaveConfig();

};

// BUG update Body class counter after importing a world.
