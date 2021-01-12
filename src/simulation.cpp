#include "simulation.hpp"

Simulation::Simulation(GLFWwindow *window, const Gui &gui) :
        m_window(window), m_gui(gui)
{
    m_vblayout.Push<float>(2);
    m_vblayout.Push<float>(1);
    m_vblayout.Push<float>(2);
    m_va.AddBuffer(m_vb, m_vblayout);

    ShaderSources sources = Shader::GetShaderSources("vertex.glsl", "fragment.glsl");
    m_shader = Shader(sources);
    m_shader.Bind();
    m_shader.SetUniformMat4f("u_MVP", MVP());
}

Simulation::~Simulation() {
}

void Simulation::Step() {
    m_world.Step();


}

void Simulation::Render() {
    m_vb.Renew(WorldVbData(), WorldVbSize());
    m_ib.Renew(WorldIbData(), WorldIbSize());
    m_va.Renew(m_vb, m_vblayout);

    m_renderer.Clear();
    m_shader.Bind();
    m_shader.SetUniformMat4f("u_MVP", MVP());
    m_renderer.Draw(m_va, m_ib, m_shader);
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

void Simulation::WorldAddBody(std::string id, glm::vec3 position, float radius,
    float mass, glm::vec3 velocity) {
    m_world.AddBody(id, position.x, position.y, mass, radius, velocity.x, velocity.y);
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
