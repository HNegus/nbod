#pragma once

#include "utils.hpp"

#include "vertexbufferlayout.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"

#include "renderer.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "world.hpp"
#include "simdata.hpp"
#include "scene.hpp"

class Simulation {

private:
    GLFWwindow *m_window;
    const Gui &m_gui;
    const Renderer m_renderer;
    Camera m_camera;
    World m_world;

    bool m_show_gui = true;
    bool m_run_simulation = false;
    // const Gui &m_gui;

    VertexArray m_va;
    VertexBuffer m_vb;
    IndexBuffer m_ib;
    VertexBufferLayout m_vblayout;
    Shader m_shader;

    SimData m_config;
public:

    Simulation(GLFWwindow *window, const Gui &gui);
    // Simulation(Camera camera, Gui gui, Renderer renderer) : m_camera(camera),
        // m_gui(gui), m_renderer(renderer) {};
    ~Simulation();


    void Init();
    void Step();


    void CameraFit();
    void CameraZoom(const int direction);
    void CameraCenter();
    void CameraMove(const glm::vec3 translation);
    void CameraSetCenter(glm::vec3 center);
    void CameraInfo();


    // TODO rename without world
    std::vector<Body*> WorldBodies() { return m_world.Bodies(); };

    void WorldAddBody();
    void WorldAddBody(std::string name, glm::vec3 position, glm::vec3 force,
                      float radius, float mass);
    void WorldRemoveBody(unsigned int id);

    void Render();
    void RenderGui();
    ImVec2 ShowMenu();

    void ShowMenuFile();
    void ShowDebug();
    void ShowDebug2();
    void ShowConfig();

    void Save(std::string scene_name);
    void Load(std::string scene_name);
    
    void GuiToggle();
    void TogglePlay();
    bool Running() const { return m_run_simulation; };

    // TODO remove
    void Do() { m_world.Do(); };
    glm::mat4 MVP() const { return m_camera.MVP(); };
    float* WorldVbData() { return m_world.vbdata(); };
    unsigned int WorldVbSize() { return m_world.vbsize(); };
    unsigned int* WorldIbData() { return m_world.ibdata(); };
    unsigned int WorldIbSize() { return m_world.ibsize(); };


};
