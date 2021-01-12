#include "simulation.hpp"

Simulation::Simulation(GLFWwindow *window, const Gui &gui) : m_window(window), m_gui(gui) {

}

Simulation::~Simulation() {
}

void Simulation::CameraZoom(const int direction) {
    m_camera.Zoom(direction);
}

void Simulation::CameraCenter() {
    m_camera.Center();
}

void Simulation::CameraSetCenter(glm::vec3 center) {
    m_camera.SetCenter(center);
}

void Simulation::CameraMove(const glm::vec3 translation) {
    m_camera.Move(translation);
}


void Simulation::GuiRender() {
    if (m_show_gui) {
        m_gui.NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Checkbox("Play", &m_run_simulation);

        m_gui.Render();
    }
}

void Simulation::GuiToggle() {
    m_show_gui = !m_show_gui;
}

void Simulation::TogglePlay() {
    m_run_simulation = !m_run_simulation;
}
