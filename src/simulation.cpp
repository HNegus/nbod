#include "simulation.hpp"

/************************* Initialization and setup. *************************/
Simulation::Simulation(GLFWwindow *window, Gui *gui) :
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
    std::filesystem::create_directory(SCENE_DIR);
    std::filesystem::create_directory(SAVE_DIR);
    std::filesystem::create_directory(LOG_DIR);
    LoadScene("default");
    if (m_config.bodies.size() > 0)
        CameraSetCenter(m_config.bodies[0]->GetPosition());
    CameraFit();
}

/* Initialize OpenGL buffers and shaders. */
void Simulation::InitBodyBuffers() {

    m_vblayout_bodies.Push<float>(2);
    m_vblayout_bodies.Push<float>(1);
    m_vblayout_bodies.Push<float>(2);
    m_vblayout_bodies.Push<float>(4);

    m_va_bodies.Bind();
    m_va_bodies.AddBuffer(m_vb_bodies, m_vblayout_bodies);

    ShaderSources sources = Shader::GetShaderSources(SHADER_DIR + BODY_SHADER_VERT,
                                                     SHADER_DIR + BODY_SHADER_FRAG);
    m_shader_bodies.Renew(sources);
}

/* Initialize OpenGL buffers and shaders. */
void Simulation::InitHistoryBuffers() {

    m_vblayout_history_positions.Push<float>(2);
    m_vblayout_history_colors.Push<unsigned char>(4);

    m_va_history.Bind();
    m_va_history.AddBuffer(m_vb_history_positions, 0, m_vblayout_history_positions);
    m_va_history.AddBuffer(m_vb_history_colors, 1, m_vblayout_history_colors);

    ShaderSources sources = Shader::GetShaderSources(SHADER_DIR + HIST_SHADER_VERT,
                                                     SHADER_DIR + HIST_SHADER_FRAG);
    m_shader_history.Renew(sources);
}

/************************* Manage simulation. *************************/
/* Add body to world. */
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

void Simulation::WorldRemoveBody(unsigned int id) {
    m_config.DeregisterBody(id);
    m_world.RemoveBody(id);
}

void Simulation::WorldClearHistory() {
    m_world.ClearBodyHistories();
}

/* Fit camera to include all bodies on the screen. */
void Simulation::CameraFit() {
    glm::vec2 lbound(0.0f), rbound(0.0f);

    // Loop over bodies to determine bounds
    for (Body *body: m_config.bodies) {
        vec3 position = body->GetPosition();

        // Add slight offset so bodies are not on the edge of the screen.
        if (position.x < lbound.x) lbound.x = 1.1 * (position.x - body->GetRadius());
        if (position.x > rbound.x) rbound.x = 1.1 * (position.x + body->GetRadius());

        if (position.y < lbound.y) lbound.y = 1.1 * (position.y - body->GetRadius());
        if (position.y > rbound.y) rbound.y = 1.1 * (position.y + body->GetRadius());

    }

    // Fit camera to bounds.
    m_camera.Fit(lbound, rbound);
}

/* Zoom camera in (direction = 1) or out (direction = -1). */
void Simulation::CameraZoom(int direction) {
    m_camera.Zoom(direction);
}

/* Set the center position of the camera. */
void Simulation::CameraSetCenter(vec3 center) {
    m_camera.SetCenter(center);
}

/* Move camera to center position. */
void Simulation::CameraCenter() {
    m_camera.Center();
}

/* Move camera to the location. */
void Simulation::CameraMove(vec3 translation) {
    m_camera.Move(translation);
}

/* Print information about the camera to stdout. */
void Simulation::CameraInfo() {
    m_camera.Info();
}

/* Advance simulaion by one step. */
void Simulation::Step() {

    // Initialize if this is the first step.
    if (m_config.initialize_world) {
        m_world.Init();
        m_config.initialize_world = false;
    }


    m_world.Step();


    if (m_config.logging) {
        m_logger.Log();
    }

    if (m_config.headless) return;

    // Update camera position if tracking is enabled.
    if (m_config.track_body) {
        CameraSetCenter(m_config.bodies[m_config.track_body_idx]->GetPosition());
    }

    // Update camera if auto resizing is enabled.
    if (m_config.auto_resize_camera) {
        CameraFit();
    }

    // Advance timer.
    m_config.time_current = m_world.GetTime();
}

/* Remove all bodies and reset camera and configuration. */
void Simulation::Clear() {
    m_world.Clear();
    m_config.Clear();
    m_camera.Clear();
}

/* Save simulation. */
void Simulation::Save(std::string scene_name) {

    if (scene_name.length() == 0) {
        return;
    }

    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.Save();
}

/* Load simulation. */
void Simulation::LoadScene(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.LoadScene();
    CameraFit();
}

void Simulation::LoadSceneHeadless(std::string scene_name) {
    std::ifstream ifs;
    std::filesystem::path path {SCENE_DIR + scene_name + "/world.cfg" };
    if (std::filesystem::exists(path)) {
        ifs.open(path);
        ifs >> m_world;
    }

    std::vector<Body *> bodies = m_world.GetBodies();
    int max_id = -1;
    int id;

    for (Body *body: bodies) {
        id = body->GetID();

        if (id > max_id) {
            max_id = (int) id;
        }
    }

    std::cout << m_world;

    Body::SetIDCounter(++max_id);

    m_config.headless = true;
    strcpy(m_config.scene_name, scene_name.c_str());

};


void Simulation::LoadSave(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.LoadSave();
    CameraFit();
}

void Simulation::StartLogging() {
    m_logger.Start(m_config.scene_name, &m_world);
    m_config.logging = true;
}

void Simulation::StopLogging() {
    m_logger.Close();
    m_config.logging = false;
}

/* Functions to enable shortcuts. */
void Simulation::GuiToggle() {
    m_config.show_gui = !m_config.show_gui;
}

void Simulation::TogglePlay() {
    m_config.run_simulation = !m_config.run_simulation;
}

void Simulation::HistoryToggle() {
    m_config.show_history = !m_config.show_history;
}

void Simulation::TrackToggle() {
    m_config.track_body = !m_config.track_body;
}

void Simulation::LimitToggle() {
    m_config.limit_framerate = !m_config.limit_framerate;
}

void Simulation::VariableTimeStepToggle() {
    m_config.variable_dt = !m_config.variable_dt;
}

/************************* Rendering. **************************/
/* Render scene/GUI. */
void Simulation::Render() {
    m_renderer.Clear();
    RenderWorld();
    RenderGui();
}

/* Render scene. */
void Simulation::RenderWorld() {

    if (m_config.limit_framerate) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    m_world.m_variable_dt = m_config.variable_dt;
    // TODO Put this in SimData.Update()
    m_config.dt = m_world.GetDeltaTime();

    // Bind body buffers and draw
    m_va_bodies.Bind();
    m_world.SetBodiesVb(m_vb_bodies);
    m_world.SetBodiesIb(m_ib_bodies);

    m_shader_bodies.Bind();
    m_shader_bodies.SetUniformMat4f("u_MVP", m_camera.MVP());

    m_renderer.Draw(m_va_bodies, m_ib_bodies, m_shader_bodies);

    m_va_bodies.UnBind();

    // Bind history buffers and draw.
    if (!m_config.show_history) return;

    m_va_history.Bind();
    m_world.SetBodiesHistoryPositionsVb(m_vb_history_positions);
    m_world.SetBodiesHistoryColorsVb(m_vb_history_colors);

    m_world.SetBodiesHistoryIb(m_ib_history);

    m_shader_history.Bind();
    m_shader_history.SetUniformMat4f("u_MVP", m_camera.MVP());
    m_renderer.DrawLineStrip(m_va_history, m_ib_history, m_shader_history);
}

/* Render Gui. */
void Simulation::RenderGui() {

    if (!m_config.show_gui) return;

    m_config.Update();

    m_gui->NewFrame();

    // Menu bar
    ImVec2 menu_size = ShowGuiMenu();
    int screen_w = m_camera.ScreenWidth();
    int screen_h = m_camera.ScreenHeight();
    int window_w = 350;


    if (!m_config.logging) {

        // World editing
        ImGui::SetNextWindowPos(ImVec2(0, menu_size.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(window_w, screen_h), ImGuiCond_Once);
        ShowGuiControl();

        // Configuration
        ImGui::SetNextWindowPos(ImVec2(screen_w - window_w, menu_size.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(window_w, 2 * (screen_h / 3)), ImGuiCond_Once);
        ShowGuiConfig();
    }
    // World info
    ImGui::SetNextWindowPos(ImVec2(screen_w - window_w, menu_size.y + 2 * (screen_h / 3)), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window_w, screen_h / 3), ImGuiCond_Once);
    ShowGuiInfo();



    m_gui->Render();
}

/* Helper function to display help markers in GUI. */
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

/* Gui for controling bodies/camera. */
void Simulation::ShowGuiControl() {

    Body *body;
    real *radius, *mass;
    vec3 *position, *velocity;
    Color *color;
    char* name;

    if (!ImGui::Begin("Body/camera control")) {
        ImGui::End();
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;


    // Edit existing bodies.
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Bodies", ImGuiTreeNodeFlags_None)) {
        ImGui::SameLine(); HelpMarker("Format: <id> <name>");


        for (std::size_t i = 0; i < m_config.bodies.size(); i++) {

            body = m_config.bodies[i];
            if (ImGui::TreeNode((void*)(intptr_t) body->GetID(), "%s", body->GetIDName().c_str())) {

                radius = body->RadiusPtr();
                mass = body->MassPtr();
                position = body->PositionPtr();
                velocity = body->VelocityPtr();

                ImGui::Spacing();

                ImGui::Text("Coordinates:");
                ImGui::InputScalar("x ",   ImGuiDataType_Real,  &position->x, &m_config.delta_position);
                ImGui::InputScalar("y ",   ImGuiDataType_Real,  &position->y, &m_config.delta_position);
                ImGui::InputScalar("z ",   ImGuiDataType_Real,  &position->z, &m_config.delta_position);

                ImGui::Spacing();

                ImGui::Text("Velocity:");
                ImGui::InputScalar("x",   ImGuiDataType_Real,  &velocity->x, &m_config.delta_velocity);
                ImGui::InputScalar("y",   ImGuiDataType_Real,  &velocity->y, &m_config.delta_velocity);
                ImGui::InputScalar("z",   ImGuiDataType_Real,  &velocity->z, &m_config.delta_velocity);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Text("Radius:");
                ImGui::InputScalar("m", ImGuiDataType_Real, (void *) radius, &m_config.delta_radius);

                ImGui::Spacing();

                ImGui::Text("Mass:");
                ImGui::InputScalar("kg", ImGuiDataType_Real, mass, &m_config.delta_mass);

                ImGui::Text("Color:");
                color = body->ColorPtr();
                ImGui::ColorEdit4("##RefColor", (float*) color, 0);

                ImGui::Spacing();

                if (ImGui::Button("Focus")) {
                    m_camera.SetCenter(body->GetPosition());
                    m_camera.Center();
                }

                ImGui::Spacing();

                if (ImGui::Button("Delete")) {
                    WorldRemoveBody(body->GetID());
                }

                ImGui::TreePop();
            }
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    // Add new body.
    ImGui::Text("Add a new body");
    ImGui::Spacing();

    body = &m_config.new_body;
    name = m_config.new_body_name;
    radius = body->RadiusPtr();
    mass = body->MassPtr();

    position = body->PositionPtr();
    velocity = body->VelocityPtr();

    ImGui::InputTextWithHint(" ", "Enter name", name, 64);
    body->SetName(name);

    ImGui::Spacing();

    ImGui::Text("Coordinates:");
    ImGui::InputScalar("x ",   ImGuiDataType_Real,  &position->x, &m_config.delta_position);
    ImGui::InputScalar("y ",   ImGuiDataType_Real,  &position->y, &m_config.delta_position);
    ImGui::InputScalar("z ",   ImGuiDataType_Real,  &position->y, &m_config.delta_position);


    ImGui::Spacing();

    ImGui::Text("Velocity:");
    ImGui::InputScalar("x",   ImGuiDataType_Real,  &velocity->x, &m_config.delta_velocity);
    ImGui::InputScalar("y",   ImGuiDataType_Real,  &velocity->y, &m_config.delta_velocity);
    ImGui::InputScalar("z",   ImGuiDataType_Real,  &velocity->z, &m_config.delta_velocity);


    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Radius:");
    ImGui::InputScalar("m", ImGuiDataType_Real, (void *) radius, &m_config.delta_radius);

    ImGui::Spacing();

    ImGui::Text("Mass:");
    ImGui::InputScalar("kg", ImGuiDataType_Real, mass, &m_config.delta_mass);


    ImGui::Text("Color:");
    color = body->ColorPtr();
    ImGui::ColorEdit4("##RefColor", (float*) color, 0);

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

    // TODO resize bodies
    // if (ImGui::Button("Resize bodies")) {
    // }

    ImGui::End();
}

/* Gui to show information about simulation. */
void Simulation::ShowGuiInfo() {

    if (!ImGui::Begin("Info")) {
        ImGui::End();
        return;
    }

    vec3 position = m_config.camera_position;
    ImGui::Text("Camera position\n x: %f \n y: %f", position.x, position.y);
    ImGui::Spacing();
    ImGui::Text("Horizontal view distance:\n %f meters", m_camera.HorizontalDistance());
    ImGui::Spacing();
    ImGui::Text("Seconds per step:\n %f", m_world.GetDeltaTime());

    int days = (int) round(m_config.time_current / SECONDS_PER_DAY);
    float years = days / DAYS_PER_YEAR;
    ImGui::Text("Elapsed time: %f s", m_config.time_current);
    ImGui::Text("Elapsed time: %d days", days);
    ImGui::Text("Elapsed time: %f years", years);

    // TODO Improve logging total energy in the system
    std::vector<real> history = m_world.TotalEnergyHistory();
    std::vector<float> values(history.begin(), history.end());
    ImGui::PlotLines("Energy in system", values.data(), values.size(), 0, " ", FLT_MIN, FLT_MAX, ImVec2(0, 80.0f));



    if (m_config.logging) {
        if (ImGui::Button("Stop logging")) {
            StopLogging();
        }
    }

    ImGui::End();
}

/* Gui for tweaking simulation parameters. */
void Simulation::ShowGuiConfig() {

    if (!ImGui::Begin("Configuration")) {
        ImGui::End();
        return;
    }

    ImGui::Spacing();
    ImGui::InputText("Scene name", m_config.scene_name, IM_ARRAYSIZE(m_config.scene_name));
    std::string scene_name(m_config.scene_name);
    ImGui::Spacing();
    ImGui::Separator();


    // TODO extra options
    // ImGui::Text("Gravitational constant");
    // ImGui::InputFloat("Gravitational constant", &m_config.gravitational_constant, 0.0f, 0.0f, "%e");
    // ImGui::Text("Gravity on/off");
    // ImGui::Separator();

    ImGui::Spacing();

    ImGui::InputScalar("Delta position", ImGuiDataType_Real, &m_config.delta_position);
    ImGui::InputScalar("Delta velocity", ImGuiDataType_Real, &m_config.delta_velocity);
    ImGui::InputScalar("Delta mass", ImGuiDataType_Real, &m_config.delta_radius);
    ImGui::InputScalar("Delta radius", ImGuiDataType_Real, &m_config.delta_mass);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("History sampling resolution");
    ImGui::InputScalar(" ", ImGuiDataType_U64, &m_config.history_resolution);
    if (m_config.history_resolution != m_world.GetHistoryResolution()) {
        m_world.SetHistoryResolution(m_config.history_resolution);
    }

    if (ImGui::Button("Clear history")) {
        WorldClearHistory();
    }

    if (!m_config.variable_dt) {
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::InputScalar("Time step", ImGuiDataType_Real, &m_config.dt);
        if (m_config.dt != m_world.GetDeltaTime()) {
            m_world.SetDeltaTime(m_config.dt);
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Checkbox("Run simulation", &m_config.run_simulation);

    ImGui::Spacing();

    ImGui::Checkbox("Auto-resize camera", &m_config.auto_resize_camera);

    ImGui::Spacing();

    // TODO
    // ImGui::Checkbox("Auto-resize bodies", &m_config.auto_resize_bodies);
    // ImGui::Spacing();

    ImGui::Checkbox("Show history", &m_config.show_history);

    ImGui::Spacing();

    ImGui::Checkbox("Limit framerate", &m_config.limit_framerate);

    ImGui::Spacing();

    ImGui::Checkbox("Variable timestep", &m_config.variable_dt);

    ImGui::Spacing();
    ImGui::Checkbox("Enable tracking", &m_config.track_body);

    if (m_config.bodies.size() > 0) {

        char *current = (char*) m_config.bodies[m_config.track_body_idx]->GetIDName().c_str();
        if (ImGui::BeginCombo("Body to track", current, 0))
        {
            for (int n = 0; n < (int) m_config.bodies.size(); n++)
            {
                const bool is_selected = (m_config.track_body_idx == n);
                if (ImGui::Selectable(m_config.bodies[n]->GetIDName().c_str(), is_selected))
                    m_config.track_body_idx = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }


    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("Step Once")) {
        Step();
    }

    ImGui::Spacing();
    if (scene_name.length() != 0 && ImGui::Button("Start logging")) {
        StartLogging();
    }


    ImGui::End();
}

/* Top menu bar. */
ImVec2 Simulation::ShowGuiMenu() {

    ImVec2 menu_size;
    if (ImGui::BeginMainMenuBar())
    {
        menu_size = ImGui::GetWindowSize();
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return menu_size;
}

/* Top menu bar options. */
void Simulation::ShowMenuFile()
{
    if (ImGui::MenuItem("New")) {
        Clear();
    }

    if (ImGui::BeginMenu("Load scene")) {
        // std::vector<std::string> scenes;
        std::string scene;

        if (std::filesystem::exists(SCENE_DIR)) {
            for (const auto &entry: std::filesystem::directory_iterator(SCENE_DIR)) {
                scene = entry.path().string().substr(10, -1);
                if (ImGui::MenuItem(scene.c_str())) {
                    LoadScene(scene);
                }
            }
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Load save")) {
        // std::vector<std::string> scenes;
        std::string scene;
        if (std::filesystem::exists(SAVE_DIR)) {
            for (const auto &entry: std::filesystem::directory_iterator(SAVE_DIR)) {
                scene = entry.path().string().substr(9, -1);
                if (ImGui::MenuItem(scene.c_str())) {
                    LoadSave(scene);
                }
            }
        }

        ImGui::EndMenu();
    }

    ImGui::Button("Save as");
    if (ImGui::BeginPopupContextItem("Save"))
    {
        ImGui::Text("Saves with the same name will be overwritten!");
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::InputTextWithHint(" ", "Enter name", m_config.scene_name, IM_ARRAYSIZE(m_config.scene_name));
        std::string scene_name(m_config.scene_name);

        if (ImGui::Button("Save")) {
            Save(scene_name);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    ImGui::OpenPopupContextItem("Save", 0);



}
