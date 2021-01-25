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
#include "logger.hpp"

class Simulation {

private:
    GLFWwindow *m_window;
    const Gui &m_gui;
    const Renderer m_renderer;
    Logger m_logger;
    Camera m_camera;
    World m_world;

    VertexArray m_va_bodies, m_va_history;
    VertexBuffer m_vb_bodies, m_vb_history_positions, m_vb_history_colors;
    IndexBuffer m_ib_bodies, m_ib_history;
    VertexBufferLayout m_vblayout_bodies, m_vblayout_history_positions, m_vblayout_history_colors;
    Shader m_shader_bodies, m_shader_history;

    SimData m_config;

public:

    Simulation(GLFWwindow *window, const Gui &gui);
    ~Simulation() {};

    void Init();
    void InitBodyBuffers();
    void InitHistoryBuffers();

    void WorldAddBody();
    void WorldAddBody(Body bodyToAdd);
    void WorldAddBody(std::string name, vec3 position, vec3 force,
                      real radius, real mass);
    void WorldAddBody(std::string name, vec3 position, vec3 force,
                    real radius, real mass, Color color);
    void WorldRemoveBody(unsigned int id);

    void CameraFit();
    void CameraZoom(int direction);
    void CameraSetCenter(vec3 center);
    void CameraCenter();
    void CameraMove(vec3 translation);
    void CameraInfo();

    void GuiToggle();
    void TogglePlay();
    void HistoryToggle();
    void TrackToggle();
    void LimitToggle();
    void VariableTimeStepToggle();
    bool Running() const { return m_config.run_simulation; };

    void Step();
    void Clear();

    void Save(std::string scene_name);
    void LoadScene(std::string scene_name);
    void LoadSave(std::string scene_name);
    void StartLogging();
    void StopLogging();

    void Render();
    void RenderWorld();
    void RenderGui();

    ImVec2 ShowGuiMenu();
    void ShowMenuFile();
    void ShowGuiControl();
    void ShowGuiInfo();
    void ShowGuiConfig();



};
