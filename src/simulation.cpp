#include "simulation.hpp"

/* Initialization, setup and management. */
Simulation::Simulation(GLFWwindow *window, const Gui &gui) :
        m_window(window), m_gui(gui)
{

    m_config.RegisterCamera(&m_camera);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.WindowRounding = 0;

    Init();
}



void Simulation::Init() {

    InitBodyBuffers();
    InitHistoryBuffers();

}

void Simulation::InitBodyBuffers() {

    m_vb_bodies.Bind();
    m_ib_bodies.Bind();

    m_world.SetBodiesVb(m_vb_bodies);
    m_world.SetBodiesIb(m_ib_bodies);

    m_vblayout_bodies.Push<float>(2);
    m_vblayout_bodies.Push<float>(1);
    m_vblayout_bodies.Push<float>(2);

    m_va_bodies.Bind();
    m_va_bodies.AddBuffer(m_vb_bodies, m_vblayout_bodies);

    // TOOD rename shaders
    ShaderSources sources = Shader::GetShaderSources("vertex.glsl", "fragment.glsl");
    m_shader_bodies.Renew(sources);
    m_shader_bodies.SetUniformMat4f("u_MVP", MVP());
    m_shader_bodies.Bind();

    m_renderer.Draw(m_va_bodies, m_ib_bodies, m_shader_bodies);
    m_va_bodies.UnBind();
}

void Simulation::InitHistoryBuffers() {

    m_va_history.Bind();

    m_vb_history_positions.Bind();
    m_world.SetBodiesHistoryPositionsVb(m_vb_history_positions);

    m_vb_history_colors.Bind();
    m_world.SetBodiesHistoryColorsVb(m_vb_history_colors);

    m_ib_history.Bind();
    m_world.SetBodiesHistoryIb(m_ib_history);

    m_vblayout_history_positions.Push<float>(2);
    m_vblayout_history_colors.Push<unsigned char>(4);

    m_va_history.Bind();
    m_va_history.AddBuffer(m_vb_history_positions, 0, m_vblayout_history_positions);
    m_va_history.AddBuffer(m_vb_history_colors, 1, m_vblayout_history_colors);

    // TODO rename shaders
    ShaderSources sources = Shader::GetShaderSources("lines.vert", "lines.frag");
    m_shader_history.Renew(sources);
    m_shader_history.SetUniformMat4f("u_MVP", MVP());
    m_shader_history.Bind();

    m_renderer.DrawLineStrip(m_va_history, m_ib_history, m_shader_history);
}

void Simulation::WorldAddBody() {
    Body* body = m_world.AddBody();
    m_config.RegisterBody(body);
}

void Simulation::WorldAddBody(std::string name,
                              glm::vec3 position, glm::vec3 velocity,
                              float radius, float mass)
{
    Body *body =  m_world.AddBody(name, position, velocity, radius, mass);
    m_config.RegisterBody(body);
}


/* Camera control. */
void Simulation::CameraFit() {
    glm::vec2 lbound(0.0f), rbound(0.0f);
    // TODO extract
    for (Body *body: m_world.Bodies()) {
        glm::vec3 position = body->GetPosition();
        if (position.x < lbound.x) lbound.x = 1.1 * (position.x - body->GetRadius());
        if (position.x > rbound.x) rbound.x = 1.1 * (position.x + body->GetRadius());

        if (position.y < lbound.y) lbound.y = 1.1 * (position.y - body->GetRadius());
        if (position.y > rbound.y) rbound.y = 1.1 * (position.y + body->GetRadius());

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
    CameraFit();
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
    if (m_config.auto_resize_camera) {
        CameraFit();
    }
}


/* Rendering scene/GUI. */
void Simulation::Render() {
    m_va_bodies.Bind();
    m_world.SetBodiesVb(m_vb_bodies);
    m_world.SetBodiesIb(m_ib_bodies);


    m_shader_bodies.Bind();
    m_shader_bodies.SetUniformMat4f("u_MVP", MVP());

    m_renderer.Clear();
    m_renderer.Draw(m_va_bodies, m_ib_bodies, m_shader_bodies);

    m_va_bodies.UnBind();

    m_va_history.Bind();
    m_world.SetBodiesHistoryPositionsVb(m_vb_history_positions);
    m_world.SetBodiesHistoryColorsVb(m_vb_history_colors);

    m_world.SetBodiesHistoryIb(m_ib_history);

    m_shader_history.Bind();
    m_shader_history.SetUniformMat4f("u_MVP", MVP());
    m_renderer.DrawLineStrip(m_va_history, m_ib_history, m_shader_history);

    RenderGui();
}

void Simulation::Clear() {
    m_world.Clear();
    m_config.Clear();
    m_camera.Clear();
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
    m_config.show_gui = !m_config.show_gui;
}

void Simulation::TogglePlay() {
    m_config.run_simulation = !m_config.run_simulation;
}



/****************** GUI rendering code ***********************/
void Simulation::RenderGui() {

    if (!m_config.show_gui) return;

    m_config.Update();

    m_gui.NewFrame();

    ImGui::ShowDemoWindow();


    ImVec2 menu_size = ShowMenu();

    ImGui::SetNextWindowPos(ImVec2(0, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, m_camera.ScreenHeight()), ImGuiCond_Once);

    ShowDebug();

    ShowDebug2();

    ImGui::SetNextWindowPos(ImVec2(m_camera.ScreenWidth() - 400, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, m_camera.ScreenHeight() / 2), ImGuiCond_Once);

    ShowConfig();

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
    if (ImGui::MenuItem("New")) {
        Clear();
    }

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
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {

        // TODO Extract
        Body *body;
        //
        // TODO extract to configuration window



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
                ImGui::InputScalar("x ",   ImGuiDataType_Float,  &position->x, &m_config.delta_position);
                ImGui::InputScalar("y ",   ImGuiDataType_Float,  &position->y, &m_config.delta_position);


                ImGui::Text("Velocity:");
                ImGui::InputScalar("x",   ImGuiDataType_Float,  &velocity->x, &m_config.delta_velocity);
                ImGui::InputScalar("y",   ImGuiDataType_Float,  &velocity->y, &m_config.delta_velocity);

                ImGui::Spacing();
                ImGui::Spacing();

                // TODO sliders or no?
                ImGui::Text("Radius:");
                ImGui::InputScalar("m", ImGuiDataType_Float, (void *) radius, &m_config.delta_radius);

                ImGui::Spacing();

                ImGui::Text("Mass:");
                ImGui::InputScalar("kg", ImGuiDataType_Float, mass, &m_config.delta_mass);


                ImGui::Text("Color");

                if (ImGui::Button("Focus")) {
                    m_camera.SetCenter(body->GetPosition());
                    m_camera.Center();
                };
                ImGui::TreePop();
            }
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Add world")) {
        WorldAddBody();
    }

    ImGui::Spacing();

    if (ImGui::Button("Fit camera")) {
        CameraFit();
    }


    ImGui::Spacing();

    // TODO
    if (ImGui::Button("Resize bodies")) {

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
    ImGui::Text("Horizontal view distance %f meters", m_camera.HorizontalDistance());
    ImGui::Text("Fps");
    ImGui::Text("Time");
    ImGui::End();

}

void Simulation::ShowConfig() {

    if (!ImGui::Begin("Configuration")) {
        ImGui::End();
        return;
    }

    ImGui::Text("World parameters");
    ImGui::Spacing();
    ImGui::Text("Gravitational constant");
    ImGui::Text("Gravity on/off");

    ImGui::Separator();

    // TODO Maximum position/velocity??
    ImGui::Text("Maximum velocity");

    float *max_mass = &m_config.max_mass;
    float *max_radius = &m_config.max_radius;

    ImGui::InputScalar("Maximum mass", ImGuiDataType_Float, max_mass);
    ImGui::InputScalar("Maximum radius", ImGuiDataType_Float, max_radius);

    ImGui::Spacing();

    float *delta_position = &m_config.delta_position;
    float *delta_velocity = &m_config.delta_velocity;
    float *delta_radius = &m_config.delta_radius;
    float *delta_mass = &m_config.delta_mass;


    ImGui::InputScalar("delta position", ImGuiDataType_Float, delta_position);
    ImGui::InputScalar("delta velocity", ImGuiDataType_Float, delta_velocity);
    ImGui::InputScalar("delta mass", ImGuiDataType_Float, delta_mass);
    ImGui::InputScalar("delta radius", ImGuiDataType_Float, delta_radius);

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Checkbox("Run simulation", &m_config.run_simulation);
    ImGui::Checkbox("Auto-resize camera", &m_config.auto_resize_camera);
    ImGui::Checkbox("Auto-resize bodies", &m_config.auto_resize_bodies);
    // ();
    ImGui::Checkbox("Track body", &m_config.track_body);
    // struct FuncHolder { static bool ItemGetter(void* data, int idx, const char** out_str) { *out_str = ((const char**)data)[idx]; return true; } };
    // static int item_current_4 = 0;
    // std::vector<std::string> names;
    // for (Body *body: m_config.bodies) {
        // names.push_back(body->Name());
    // }
    // char **items = names.data();
    // ImGui::Combo("combo 4 (function)", &item_current_4, &m_config.ComboGetter, items, m_bodies.size());



    ImGui::End();
}
