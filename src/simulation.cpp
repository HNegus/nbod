#include "simulation.hpp"

/* Initialization, setup and management. */
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

void Simulation::Init() {
    m_config.RegisterCamera(&m_camera);
    CameraFit();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.WindowRounding = 0;
}


void Simulation::WorldAddBody() {
    Body* body = m_world.AddBody();
    m_config.RegisterBody(body);
}


void Simulation::WorldAddBody(std::string name,
                              glm::vec3 position, glm::vec3 velocity,
                              float radius, float mass)
{
    // .push_back(id);
    Body *body =  m_world.AddBody(name, position, velocity, radius, mass);
    m_config.RegisterBody(body);

}


/* Camera control. */
void Simulation::CameraFit() {
    glm::vec2 lbound(0.0f), rbound(0.0f);
    // TODO extract
    for (Body *body: m_world.Bodies()) {
        glm::vec3 position = body->GetPosition();
        if (position.x < lbound.x) lbound.x = 1.01 * position.x - body->GetRadius();
        if (position.x > rbound.x) rbound.x = 1.01 * position.x + body->GetRadius();

        if (position.y < lbound.y) lbound.y = 1.01 * position.y - body->GetRadius();
        if (position.y > rbound.y) rbound.y = 1.01 * position.y + body->GetRadius();

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

/* Simulation */
void Simulation::Step() {
    m_world.Step();
}


/* Rendering scene/GUI. */
void Simulation::Render() {
    m_va.Bind();
    m_vb.Update(WorldVbData(), WorldVbSize());
    m_ib.Update(WorldIbData(), WorldIbSize());

    m_shader.Bind();
    m_shader.SetUniformMat4f("u_MVP", MVP());
    m_renderer.Clear();

    m_renderer.Draw(m_va, m_ib, m_shader);

    RenderGui();
}



void Simulation::RenderGui() {

    if (!m_show_gui) return;

    m_config.Update();

    m_gui.NewFrame();

    ImGui::ShowDemoWindow();


    ImVec2 menu_size = ShowMenu();

    ImGui::SetNextWindowPos(ImVec2(0, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, m_camera.ScreenHeight()), ImGuiCond_Always);

    ShowDebug();

    ShowDebug2();

    ImGui::SetNextWindowPos(ImVec2(m_camera.ScreenWidth() - 200, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, m_camera.ScreenHeight() / 2), ImGuiCond_Always);

    ShowConfig();

    m_world.UpdateWorld();
    m_gui.Render();

}


ImVec2 Simulation::ShowMenu() {

    ImVec2 menu_size;
    if (ImGui::BeginMainMenuBar())
    {
        menu_size = ImGui::GetWindowSize();
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        // if (ImGui::BeginMenu("Edit"))
        // {
        //     if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        //     if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        //     ImGui::Separator();
        //     if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        //     if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        //     if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        //     ImGui::EndMenu();
        // }
        ImGui::EndMainMenuBar();
    }

    return menu_size;
}

void Simulation::ShowMenuFile()
{
    // TODO load default scene
    if (ImGui::MenuItem("New")) {}

    // TODO prepare and load scenes
    if (ImGui::Button("Load scene"))
        ImGui::OpenPopup("Load scene");

    if (ImGui::Button("Save as"))
        ImGui::OpenPopup("Save as");

    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Load scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {

        std::vector<std::string> scenes;
        std::string path;

        for (const auto &entry : std::filesystem::directory_iterator("../scenes"))
            scenes.push_back(entry.path().string().substr(10, -1));

        // for (std::string &scene: scenes)
            // std::cout << scene << std::endl;

        // char *items = &scenes[0];
        char **items = new char*[scenes.size()];
        for (int i = 0; i < scenes.size(); i++) {
            items[i] = (char*) scenes[i].data();
        }
        // TODO selectables to config
        static int item_current;
        {
            ImGui::ListBox("listbox\n(single select)", &item_current, items, (size_t) scenes.size(), scenes.size() > 10 ? 10 : scenes.size());
        }
        std::cout << item_current << std::endl;
        delete[] items;
        if (ImGui::Button("Load", ImVec2(120, 0))) {
            Load(scenes[item_current]);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Save as", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Previous saves with the same name will be overwritten\n\n");
        static char buf1[64] = "";
        ImGui::InputText(" ",     buf1, 64);
        ImGui::Separator();

        // TODO guard against empty filenames
        if (ImGui::Button("Save", ImVec2(120, 0))) {
            std::string scene_name(buf1);
            Save(scene_name);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }


    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x+sz, p.y+sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }

    // Here we demonstrate appending again to the "Options" menu (which we already created above)
    // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
    // In a real code-base using it would make senses to use this feature from very different code locations.
    if (ImGui::BeginMenu("Options")) // <-- Append!
    {
        static bool b = true;
        ImGui::Checkbox("SomeOption", &b);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}



void Simulation::ShowDebug() {

    if (!ImGui::Begin("Debug")) {
        ImGui::End();
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;

    // std::cout << "!!" << ImGui::GetIO().WantCaptureMouse << std::endl;

    ImGui::Checkbox("Play", &m_run_simulation);


    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {

        // TODO Extract
        Body *body;
        //
        // TODO extract to configuration window
        float *delta_position = &m_config.delta_position;
        float *delta_velocity = &m_config.delta_velocity;
        float *delta_radius = &m_config.delta_radius;
        float *delta_mass = &m_config.delta_mass;


        ImGui::InputScalar("delta position", ImGuiDataType_Float, delta_position);
        ImGui::InputScalar("delta velocity", ImGuiDataType_Float, delta_velocity);
        ImGui::InputScalar("delta mass", ImGuiDataType_Float, delta_mass);
        ImGui::InputScalar("delta radius", ImGuiDataType_Float, delta_radius);




        for (std::size_t i = 0; i < m_config.bodies.size(); i++) {

            body = m_config.bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t) body->ID(), "%s", body->Name().c_str())) {



                float *radius = body->RadiusPtr();
                float *mass = body->MassPtr();
                glm::vec3 *position = body->PositionPtr();
                glm::vec3 *velocity = body->VelocityPtr();

                // TODO

                const float max_mass = m_config.max_mass;
                const float max_radius = m_config.max_radius;


                ImGui::Text("Coordinates:");
                ImGui::InputScalar("x",   ImGuiDataType_Float,  &position->x, delta_position);
                ImGui::InputScalar("y",   ImGuiDataType_Float,  &position->y, delta_position);



                ImGui::Text("Velocity:");
                ImGui::InputScalar("x",   ImGuiDataType_Float,  &velocity->x, delta_velocity);
                ImGui::InputScalar("y",   ImGuiDataType_Float,  &velocity->x, delta_velocity);

                ImGui::Spacing();
                ImGui::Spacing();

                // TODO sliders or no?
                ImGui::Text("Radius:");
                ImGui::InputScalar("m", ImGuiDataType_Float, (void *) radius, delta_radius);
                // ImGui::SliderScalar("Radius (drag value)", ImGuiDataType_Float, radius, &FLOAT_ZERO, &max_radius, "%e", 1.0f);

                ImGui::Spacing();

                ImGui::Text("Mass:");
                ImGui::InputScalar("kg", ImGuiDataType_Float, mass, delta_mass);
                // // ImGui::SliderScalar("",ImGuiDataType_Float, mass, &FLOAT_ZERO, &max_mass, "%e", 1.0f);
                //
                //
                // ImGui::Spacing();
                //
                //
                // ImGui::Text("Color:");
                // ImGui::Spacing();
                //
                //
                // // ImGui::InputFloat3("Position xyz (enter values)", (float *) position);
                // // ImGui::DragFloat3("Position xyz (drag numbers)", (float *) position, position_step, min_pos, max_pos);
                //

                ImGui::TreePop();
            }


        }


    }


    if (ImGui::Button("Add world")) {
        WorldAddBody();
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

void Simulation::ShowConfig() {

    if (!ImGui::Begin("Configuration")) {
        ImGui::End();
        return;
    }

    // TODO Maximum position??
    ImGui::Text("Maximum mass");
    ImGui::Text("Maximum radius");
    ImGui::Text("Maximum velocity");


    ImGui::Text("delta position");
    ImGui::Text("delta velocity");
    ImGui::Text("delta mass");
    ImGui::Text("delta radius");





    ImGui::End();
}

void Simulation::Save(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.Save();
}

void Simulation::Load(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.Load();
}

void Simulation::GuiToggle() {
    m_show_gui = !m_show_gui;
}

void Simulation::TogglePlay() {
    m_run_simulation = !m_run_simulation;
}
