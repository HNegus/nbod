#include "simulation.hpp"

Simulation::Simulation(GLFWwindow *window, const Gui &gui) :
        m_window(window), m_gui(gui)
{
    m_va.Bind();
    m_vb.Renew(WorldVbData(), WorldVbSize());
    m_ib.Renew(WorldIbData(), WorldIbSize());
    m_vblayout.Push<float>(2);
    m_vblayout.Push<float>(1);
    m_vblayout.Push<float>(2);
    m_va.AddBuffer(m_vb, m_vblayout);

    ShaderSources sources = Shader::GetShaderSources("vertex.glsl", "fragment.glsl");
    m_shader.Renew(sources);
    m_shader.SetUniformMat4f("u_MVP", MVP());
    m_shader.Bind();
    m_renderer.Draw(m_va, m_ib, m_shader);

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

    m_shader.Bind();
    m_shader.SetUniformMat4f("u_MVP", MVP());
    m_renderer.Clear();

    m_renderer.Draw(m_va, m_ib, m_shader);

    GuiRender();
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

void Simulation::WorldAddBody(std::string name, glm::vec3 position, float radius,
                              float mass, glm::vec3 velocity) {

    // .push_back(id);

    m_world.AddBody(name, position.x, position.y, mass, radius, velocity.x, velocity.y);
}



void Simulation::GuiRender() {
    if (!m_show_gui) return;
    m_gui.NewFrame();

    ImGui::ShowDemoWindow();

    if (!ImGui::Begin("Debug 1")) {
        ImGui::End();
        return;
    }
    ImGui::Checkbox("Play", &m_run_simulation);


    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None))
    {
        std::vector<Body*> bodies = m_world.Bodies();
        Body *body;

        for (std::size_t i = 0; i < bodies.size(); i++) {

            body = bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t)i, "%s", body->Name().c_str())) {
                ImGui::Text("Blabla", i);
                ImGui::TreePop();
            }
        }


    }


    // ImGui::TreePop();
    // int but = 0;
    if (ImGui::Button("Add world")) {
        std::cout << "Add world" << std::endl;
    }

    ImGui::End();

    if (!ImGui::Begin("Debug 2")) {
        ImGui::End();
        return;
    }
    bool a;
    ImGui::Checkbox("Play2", &a);


    ImGui::End();


    m_gui.Render();

}

void Simulation::GuiToggle() {
    m_show_gui = !m_show_gui;
}

void Simulation::TogglePlay() {
    m_run_simulation = !m_run_simulation;
}
