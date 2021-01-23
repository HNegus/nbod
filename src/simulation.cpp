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
    Load("default");
    CameraSetCenter(m_world.Bodies()[0]->GetPosition());
    CameraFit();
}

void Simulation::InitBodyBuffers() {

    m_vblayout_bodies.Push<float>(2);
    m_vblayout_bodies.Push<float>(1);
    m_vblayout_bodies.Push<float>(2);
    m_vblayout_bodies.Push<float>(4);

    m_va_bodies.Bind();
    m_va_bodies.AddBuffer(m_vb_bodies, m_vblayout_bodies);

    // TOOD rename shaders
    ShaderSources sources = Shader::GetShaderSources("vertex.glsl", "fragment.glsl");
    m_shader_bodies.Renew(sources);
}

void Simulation::InitHistoryBuffers() {

    m_vblayout_history_positions.Push<float>(2);
    m_vblayout_history_colors.Push<unsigned char>(4);

    m_va_history.Bind();
    m_va_history.AddBuffer(m_vb_history_positions, 0, m_vblayout_history_positions);
    m_va_history.AddBuffer(m_vb_history_colors, 1, m_vblayout_history_colors);

    // TODO rename shaders
    ShaderSources sources = Shader::GetShaderSources("lines.vert", "lines.frag");
    m_shader_history.Renew(sources);
}

void Simulation::WorldAddBody() {
    Body* body = m_world.AddBody();
    m_config.RegisterBody(body);
}

void Simulation::WorldAddBody(Body bodyToAdd) {
    Body *body = m_world.AddBody(bodyToAdd);
    m_config.RegisterBody(body);
}

void Simulation::WorldAddBody(std::string name,
                              vec3 position, vec3 velocity,
                              real radius, real mass)
{
    Body *body =  m_world.AddBody(name, position, velocity, radius, mass);
    m_config.RegisterBody(body);
}

void Simulation::WorldAddBody(std::string name,
                              vec3 position, vec3 velocity,
                              real radius, real mass, Color color)
{
    Body *body =  m_world.AddBody(name, position, velocity, radius, mass, color);
    m_config.RegisterBody(body);
}


/* Camera control. */
void Simulation::CameraFit() {
    glm::vec2 lbound(0.0f), rbound(0.0f);
    // TODO extract
    for (Body *body: m_world.Bodies()) {
        vec3 position = body->GetPosition();
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

void Simulation::CameraSetCenter(vec3 center) {
    m_camera.SetCenter(center);
}

void Simulation::CameraMove(const vec3 translation) {
    m_camera.Move(translation);
}

void Simulation::CameraInfo() {
    m_camera.Info();
}

/* Simulation */
void Simulation::Step() {
    // m_world.Step();
    if (m_config.initialize_world) {
        m_world.Init();
        m_config.initialize_world = false;
    }

    m_world.Step();

    if (m_config.track_body) {
        CameraSetCenter(m_config.bodies[m_config.track_body_idx]->GetPosition());
    }
    if (m_config.auto_resize_camera) {
        CameraFit();
    }
    m_config.time_current += m_world.m_dt;
}


/* Rendering scene/GUI. */
void Simulation::Render() {

    m_renderer.Clear();
    RenderWorld();
    RenderGui();


}

void Simulation::RenderWorld() {
    m_va_bodies.Bind();
    m_world.SetBodiesVb(m_vb_bodies);
    m_world.SetBodiesIb(m_ib_bodies);


    m_shader_bodies.Bind();
    m_shader_bodies.SetUniformMat4f("u_MVP", MVP());

    m_renderer.Draw(m_va_bodies, m_ib_bodies, m_shader_bodies);

    m_va_bodies.UnBind();

    if (!m_config.show_history) return;

    m_va_history.Bind();
    m_world.SetBodiesHistoryPositionsVb(m_vb_history_positions);
    m_world.SetBodiesHistoryColorsVb(m_vb_history_colors);

    m_world.SetBodiesHistoryIb(m_ib_history);

    m_shader_history.Bind();
    m_shader_history.SetUniformMat4f("u_MVP", MVP());
    m_renderer.DrawLineStrip(m_va_history, m_ib_history, m_shader_history);
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
    CameraFit();
    m_config.initialize_world = true;
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
        ImGui::SetWindowCollapsed("File", false, 0);
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
        real sz = ImGui::GetTextLineHeight();
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



static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Simulation::ShowDebug() {

    if (!ImGui::Begin("Control world")) {
        ImGui::End();
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;

    // std::cout << "!!" << ImGui::GetIO().WantCaptureMouse << std::endl;
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {
        ImGui::SameLine(); HelpMarker("Format: <id> <name>");

        // TODO Extract
        Body *body;
        //
        // TODO extract to configuration window


        for (std::size_t i = 0; i < m_config.bodies.size(); i++) {

            body = m_config.bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t) body->ID(), "%s", body->Name().c_str())) {


                real *radius = body->RadiusPtr();
                real *mass = body->MassPtr();
                vec3 *position = body->PositionPtr();
                vec3 *velocity = body->VelocityPtr();

                // TODO

                const real max_mass = m_config.max_mass;
                const real max_radius = m_config.max_radius;

                ImGui::Spacing();

                ImGui::Text("Coordinates:");
                ImGui::InputScalar("x ",   ImGuiDataType_Real,  &position->x, &m_config.delta_position);
                ImGui::InputScalar("y ",   ImGuiDataType_Real,  &position->y, &m_config.delta_position);

                ImGui::Spacing();

                ImGui::Text("Velocity:");
                ImGui::InputScalar("x",   ImGuiDataType_Real,  &velocity->x, &m_config.delta_velocity);
                ImGui::InputScalar("y",   ImGuiDataType_Real,  &velocity->y, &m_config.delta_velocity);

                ImGui::Spacing();
                ImGui::Spacing();

                // TODO sliders or no?
                ImGui::Text("Radius:");
                ImGui::InputScalar("m", ImGuiDataType_Real, (void *) radius, &m_config.delta_radius);

                ImGui::Spacing();

                ImGui::Text("Mass:");
                ImGui::InputScalar("kg", ImGuiDataType_Real, mass, &m_config.delta_mass);


                ImGui::Text("Color:");
                Color *c = body->ColorPtr();
                ImGui::ColorEdit4("##RefColor", (float*) c, 0);

                // std::cout << (unsigned int) round(255.0f*color[0]) << std::endl;
                // c.r = (unsigned char) (color.x * 225.0f);
                // c.g = (unsigned char) (color.y * 225.0f);
                // c.b = (unsigned char) (color.z * 225.0f);
                // c.a = (unsigned char) (color.w * 225.0f);
                // body->SetColor(color);
                // std::cout << (int) (body->GetColor().r) << std::endl;
                // std::cout << std::endl;



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


    ImGui::Text("Add a new body");
    ImGui::Spacing();

    Body *body = &m_config.NewBody;
    char *name = m_config.NewBodyName;
    real* radius = body->RadiusPtr();
    real* mass = body->MassPtr();

    vec3 *position = body->PositionPtr();
    vec3 *velocity = body->VelocityPtr();

    ImGui::InputTextWithHint(" ", "Enter name", name, 64);
    body->SetName(name);

    ImGui::Spacing();

    ImGui::Text("Coordinates:");
    ImGui::InputScalar("x ",   ImGuiDataType_Real,  &position->x, &m_config.delta_position);
    ImGui::InputScalar("y ",   ImGuiDataType_Real,  &position->y, &m_config.delta_position);


    ImGui::Spacing();

    ImGui::Text("Velocity:");
    ImGui::InputScalar("x",   ImGuiDataType_Real,  &velocity->x, &m_config.delta_velocity);
    ImGui::InputScalar("y",   ImGuiDataType_Real,  &velocity->y, &m_config.delta_velocity);


    ImGui::Spacing();
    ImGui::Spacing();

    // TODO sliders or no?
    ImGui::Text("Radius:");
    ImGui::InputScalar("m", ImGuiDataType_Real, (void *) radius, &m_config.delta_radius);

    ImGui::Spacing();

    ImGui::Text("Mass:");
    ImGui::InputScalar("kg", ImGuiDataType_Real, mass, &m_config.delta_mass);


    ImGui::Text("Color:");
    Color *c = body->ColorPtr();
    ImGui::ColorEdit4("##RefColor", (float*) c, 0);

    ImGui::Spacing();

    if (ImGui::Button("Add")) {
        body->SetID(Body::GetIDCounter());
        Body::IncIDCounter();
        WorldAddBody(*body);
    }

    ImGui::Spacing();
    ImGui::Separator();
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

    vec3 position = m_config.camera_position;
    ImGui::Text("Camera position\n x: %f \n y: %f", position.x, position.y);
    ImGui::Text("Horizontal view distance %f meters", m_camera.HorizontalDistance());
    ImGui::Text("Fps");

    int days = (int) round(m_config.time_current / SECONDS_PER_DAY);
    float years = days / DAYS_PER_YEAR;
    ImGui::Text("Elapsed time: %f s", m_config.time_current);
    ImGui::Text("Elapsed time: %d days", days);
    ImGui::Text("Elapsed time: %f years", years);

    std::cout << m_world.KineticEnergy() << "\t" << m_world.PotentialEnergy() << "\t" << m_world.TotalEnergy() << std::endl;

    std::vector<real> history = m_world.TotalEnergyHistory();
    std::vector<float> values(history.begin(), history.end());
    // double values[11] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 10.0f, 5.0f, 1.0f, 0.0f, -1.0f, -10.0f};
    ImGui::PlotLines("Lines", values.data(), values.size(), 0, "text", FLT_MIN, FLT_MAX, ImVec2(0, 80.0f));


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

    real *max_mass = &m_config.max_mass;
    real *max_radius = &m_config.max_radius;

    ImGui::InputScalar("Maximum mass", ImGuiDataType_Real, max_mass);
    ImGui::InputScalar("Maximum radius", ImGuiDataType_Real, max_radius);

    ImGui::Spacing();

    real *delta_position = &m_config.delta_position;
    real *delta_velocity = &m_config.delta_velocity;
    real *delta_radius = &m_config.delta_radius;
    real *delta_mass = &m_config.delta_mass;


    ImGui::InputScalar("delta position", ImGuiDataType_Real, delta_position);
    ImGui::InputScalar("delta velocity", ImGuiDataType_Real, delta_velocity);
    ImGui::InputScalar("delta mass", ImGuiDataType_Real, delta_mass);
    ImGui::InputScalar("delta radius", ImGuiDataType_Real, delta_radius);

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Checkbox("Run simulation", &m_config.run_simulation);
    ImGui::Checkbox("Auto-resize camera", &m_config.auto_resize_camera);
    ImGui::Checkbox("Auto-resize bodies", &m_config.auto_resize_bodies);

    ImGui::Spacing();
    ImGui::Checkbox("Enable tracking", &m_config.track_body);

    if (m_config.bodies.size() > 0) {

        char *current = (char*) m_config.bodies[m_config.track_body_idx]->Name().c_str();
        if (ImGui::BeginCombo("Body to track", current, 0))
        {
            for (int n = 0; n < (int) m_config.bodies.size(); n++)
            {
                const bool is_selected = (m_config.track_body_idx == n);
                if (ImGui::Selectable(m_config.bodies[n]->Name().c_str(), is_selected))
                    m_config.track_body_idx = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

    }

    ImGui::Spacing();

    ImGui::Checkbox("Show history", &m_config.show_history);

    ImGui::Spacing();


    if (ImGui::Button("Step Once")) {
        Step();
    }

    ImGui::End();
}
