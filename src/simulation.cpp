#include "simulation.hpp"

/************************* Initialization and setup. *************************/
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
// TODO remove body
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

    // Update camera position if tracking is enabled.
    if (m_config.track_body) {
        CameraSetCenter(m_config.bodies[m_config.track_body_idx]->GetPosition());
    }

    // Update camera if auto resizing is enabled.
    if (m_config.auto_resize_camera) {
        CameraFit();
    }

    // Advance timer.
    m_config.time_current += m_world.m_dt;
}

/* Remove all bodies and reset camera and configuration. */
void Simulation::Clear() {
    m_world.Clear();
    m_config.Clear();
    m_camera.Clear();
}

/* Save simulation. */
void Simulation::Save(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.Save();
}

/* Load simulation. */
void Simulation::Load(std::string scene_name) {
    Scene scene(scene_name, m_world, m_camera, m_config);
    scene.Load();
    CameraFit();
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


/************************* Rendering. **************************/
/* Render scene/GUI. */
void Simulation::Render() {
    m_renderer.Clear();
    RenderWorld();
    RenderGui();
}

/* Render scene. */
void Simulation::RenderWorld() {

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

    m_gui.NewFrame();

    // TODO remove demo window
    ImGui::ShowDemoWindow();

    // Menu bar
    ImVec2 menu_size = ShowGuiMenu();
    int screen_w = m_camera.ScreenWidth();
    int screen_h = m_camera.ScreenHeight();
    int window_w = 300;

    // World editing
    ImGui::SetNextWindowPos(ImVec2(0, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window_w, screen_h), ImGuiCond_Once);
    ShowGuiControl();

    // World info
    ImGui::SetNextWindowPos(ImVec2(screen_w - window_w, menu_size.y + (screen_h / 2)), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window_w, screen_h / 2), ImGuiCond_Once);
    ShowGuiInfo();

    // Configuration
    ImGui::SetNextWindowPos(ImVec2(screen_w - window_w, menu_size.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window_w, screen_h / 2), ImGuiCond_Once);
    ShowGuiConfig();

    m_gui.Render();
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
            if (ImGui::TreeNode((void*)(intptr_t) body->GetID(), "%s", body->GetName().c_str())) {

                radius = body->RadiusPtr();
                mass = body->MassPtr();
                position = body->PositionPtr();
                velocity = body->VelocityPtr();

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

    body = &m_config.NewBody;
    name = m_config.NewBodyName;
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

    // TODO
    if (ImGui::Button("Resize bodies")) {

    }

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
    ImGui::Text("Horizontal view distance %f meters", m_camera.HorizontalDistance());
    ImGui::Text("Fps");

    int days = (int) round(m_config.time_current / SECONDS_PER_DAY);
    float years = days / DAYS_PER_YEAR;
    ImGui::Text("Elapsed time: %f s", m_config.time_current);
    ImGui::Text("Elapsed time: %d days", days);
    ImGui::Text("Elapsed time: %f years", years);

    // TODO logging and showing total energy.
    // std::cout << m_world.KineticEnergy() << "\t" << m_world.PotentialEnergy() << "\t" << m_world.TotalEnergy() << std::endl;

    std::vector<real> history = m_world.TotalEnergyHistory();
    std::vector<float> values(history.begin(), history.end());
    ImGui::PlotLines("Lines", values.data(), values.size(), 0, "text", FLT_MIN, FLT_MAX, ImVec2(0, 80.0f));

    ImGui::End();
}

/* Gui for tweaking simulation parameters. */
void Simulation::ShowGuiConfig() {

    if (!ImGui::Begin("Configuration")) {
        ImGui::End();
        return;
    }

    ImGui::Text("World parameters");
    ImGui::Spacing();
    ImGui::Text("Gravitational constant");
    // TODO
    // ImGui::InputFloat("Gravitational constant", &m_config.gravitational_constant, 0.0f, 0.0f, "%e");
    ImGui::Text("Gravity on/off");

    ImGui::Separator();
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

        char *current = (char*) m_config.bodies[m_config.track_body_idx]->GetName().c_str();
        if (ImGui::BeginCombo("Body to track", current, 0))
        {
            for (int n = 0; n < (int) m_config.bodies.size(); n++)
            {
                const bool is_selected = (m_config.track_body_idx == n);
                if (ImGui::Selectable(m_config.bodies[n]->GetName().c_str(), is_selected))
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

        char **items = new char*[scenes.size()];
        for (size_t i = 0; i < scenes.size(); i++) {
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
}
