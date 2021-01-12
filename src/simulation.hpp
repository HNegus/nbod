#pragma once

#include "utils.hpp"
#include "camera.hpp"
#include "gui.hpp"

class Simulation {

private:
    GLFWwindow *m_window;
    const Gui &m_gui;
    Camera m_camera;

    bool m_show_gui = true;
    bool m_run_simulation = false;
    // const Gui &m_gui;
    // const Renderer &m_renderer;

public:

    Simulation(GLFWwindow *window, const Gui &gui);
    // Simulation(Camera camera, Gui gui, Renderer renderer) : m_camera(camera),
        // m_gui(gui), m_renderer(renderer) {};
    ~Simulation();


    void CameraZoom(const int direction);
    void CameraCenter();
    void CameraMove(const glm::vec3 translation);
    void CameraSetCenter(glm::vec3 center);

    // TODO remove
    glm::mat4 MVP() const { return m_camera.MVP(); };

    void GuiRender();
    void GuiToggle();

    void TogglePlay();

    bool Running() const { return m_run_simulation; };

};
