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

void Simulation::Init() {
    m_config.RegisterCamera(&m_camera);
    CameraFit();
}

void Simulation::CameraFit() {
    glm::vec2 lbound(0.0f), rbound(0.0f);
    // TODO extract
    for (Body *body: m_world.Bodies()) {
        glm::vec3 position = body->GetPosition();
        if (position.x < lbound.x) lbound.x = 1.01 * position.x - body->Radius();
        if (position.x > rbound.x) rbound.x = 1.01 * position.x + body->Radius();

        if (position.y < lbound.y) lbound.y = 1.01 * position.y - body->Radius();
        if (position.y > rbound.y) rbound.y = 1.01 * position.y + body->Radius();

    }

    m_camera.Fit(lbound, rbound);
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

void Simulation::CameraInfo() {
    m_camera.Info();
}

void Simulation::WorldAddBody(std::string name, glm::vec3 position, float radius,
                              float mass, glm::vec3 velocity) {

    // .push_back(id);
    Body *body =  m_world.AddBody(name, position.x, position.y,
                               mass, radius, velocity.x, velocity.y);
    m_config.RegisterBody(body);

}



void Simulation::GuiRender() {

    if (!m_show_gui) return;

    m_config.Update();

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

    // std::cout << "!!" << ImGui::GetIO().WantCaptureMouse << std::endl;

    ImGui::Checkbox("Play", &m_run_simulation);


    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {

        // TODO Extract
        Body *body;
        float diff;

        for (std::size_t i = 0; i < m_config.bodies.size(); i++) {

            body = m_config.bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t)i, "%s", body->Name().c_str())) {


                float *radius = body->RadiusPtr();
                float *mass = body->MassPtr();
                const glm::vec3 *position = body->PositionPtr();


                // ImGui::Text("Blabla", i);
                const float radius_step = 0.01 * (*radius);
                const float mass_step = 0.01 * (*mass);

                const float max_mass = m_config.max_mass;
                const float max_radius = m_config.max_radius;


                const float xmin = m_config.min_position.x;
                const float xmax = m_config.max_position.x;
                diff = xmax - xmin;
                const float xstep = diff > 0 ? diff : 1.0f;


                const float ymin = m_config.min_position.x;
                const float ymax = m_config.max_position.x;
                diff = ymax - ymin;
                const float ystep = diff > 0 ? diff : 1.0f;

                ImGui::InputScalar("Radius (enter value)",   ImGuiDataType_Float, radius, &radius_step);
                ImGui::SliderScalar("Radius (drag values)", ImGuiDataType_Float, radius, &FLOAT_ZERO, &max_radius, "%e", 1.0f);

                ImGui::Spacing();


                ImGui::InputScalar("Mass (enter value)",   ImGuiDataType_Float,  mass, &mass_step);
                ImGui::SliderScalar("Mass (drag values)", ImGuiDataType_Float, mass, &FLOAT_ZERO, &max_mass, "%e", 1.0f);

                ImGui::Spacing();

                ImGui::Text("X position");
                ImGui::Text("Y position");

                ImGui::Spacing();

                // Only increase/decrease buttons?
                ImGui::Text("X velocity");
                ImGui::Text("Y velocity");

                ImGui::Spacing();


                ImGui::Text("Color");
                ImGui::Spacing();


                // ImGui::InputFloat3("Position xyz (enter values)", (float *) position);
                // ImGui::DragFloat3("Position xyz (drag numbers)", (float *) position, position_step, min_pos, max_pos);


                ImGui::TreePop();
            }
        }
    }

    if (ImGui::Button("Add world")) {
        WorldAddBody("three ", glm::vec3(0.0f), RADIUS_EARTH, MASS_MOON, glm::vec3(0.0f));
    }

    if (ImGui::Button("Fit camera")) {
        CameraFit();

    }

    ImGui::End();
}

void Simulation::ShowDebug2() {

    if (!ImGui::Begin("Debug 2")) {
        ImGui::End();
        return;
    }

    glm::vec3 position = m_config.camera_position;
    ImGui::Text("Camera position\n x: %f \n y: %f", position.x, position.y);

    ImGui::End();

}


void Simulation::GuiToggle() {
    m_show_gui = !m_show_gui;
}

void Simulation::TogglePlay() {
    m_run_simulation = !m_run_simulation;
}
