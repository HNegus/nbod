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

    ShowDebug();

    ShowDebug2();





    m_world.UpdateWorld();


    m_gui.Render();

}

void Simulation::ShowDebug() {

    if (!ImGui::Begin("Debug")) {
        ImGui::End();
        return;
    }

    ImGui::Checkbox("Play", &m_run_simulation);


    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {

        std::vector<Body*> bodies = m_world.Bodies();
        Body *body;

        for (std::size_t i = 0; i < bodies.size(); i++) {

            body = bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t)i, "%s", body->Name().c_str())) {

                float *radius = body->RadiusPtr();
                float *mass = body->MassPtr();
                glm::vec3 *position = body->PositionPtr();


                // ImGui::Text("Blabla", i);
                const float max_radius = 10*RADIUS_EARTH;
                const float radius_step = 0.01 * (*radius);

                const float max_mass = 10*MASS_EARTH;
                const float mass_step = 0.01 * (*mass);

                const float min_pos = -10*RADIUS_EARTH;
                const float max_pos = 10*RADIUS_EARTH;
                const float position_step = 0.01 * max_pos;


                ImGui::InputScalar("Radius (enter value)",   ImGuiDataType_Float,  radius, &radius_step);
                ImGui::SliderScalar("Radius (drag values)", ImGuiDataType_Float, radius, &FLOAT_ZERO, &max_radius, "%e", 2.0f);

                ImGui::Spacing();

                ImGui::InputFloat3("Position xyz (enter values)", (float *) position);
                ImGui::DragFloat3("Position xyz (drag numbers)", (float *) position, position_step, min_pos, max_pos);

                ImGui::InputScalar("Mass (enter value)",   ImGuiDataType_Float,  mass, &mass_step);
                ImGui::SliderScalar("Mass (drag values)", ImGuiDataType_Float, mass, &FLOAT_ZERO, &max_mass, "%e", 2.0f);


                ImGui::TreePop();
            }
        }
    }

    if (ImGui::Button("Add world")) {
        WorldAddBody("three ", glm::vec3(0.0f), RADIUS_EARTH, MASS_MOON, glm::vec3(0.0f));
    }

    ImGui::End();
}

void Simulation::ShowDebug2() {

    if (!ImGui::Begin("Debug 2")) {
        ImGui::End();
        return;
    }

    bool a;
    ImGui::Checkbox("Play2", &a);

    ImGui::End();

}


void Simulation::GuiToggle() {
    m_show_gui = !m_show_gui;
}

void Simulation::TogglePlay() {
    m_run_simulation = !m_run_simulation;
}
