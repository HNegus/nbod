// #define GLM_PRECISION_HIGHP_DOUBLE

#include "utils.hpp"

#include "errors.hpp"

#include "shader.hpp"
// #include "vertexbufferlayout.hpp"
// #include "vertexbuffer.hpp"
// #include "indexbuffer.hpp"
// #include "vertexarray.hpp"
#include "texture.hpp"

// #include "renderer.hpp"
#include "gui.hpp"
#include "scene.hpp"
#include "simulation.hpp"
#include "input.hpp"

#include "body.hpp"
#include "world.hpp"
// TODO remove
#include <unistd.h>
E_ErrorLevels ERROR_LEVEL = HIGH;


static GLFWwindow* init() {

    glm::vec3 v;
    std::cout << glm::to_string(v) << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "glfwInit failed." << std::endl;
        return nullptr;
    }

    /* Hint correct OpenGL version to GLFW. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);


    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* return_struct = glfwGetVideoMode(monitor);

    // window = glfwCreateWindow(1920, 1080, "Hello World", glfwGetPrimaryMonitor(), nullptr);
    window = glfwCreateWindow(return_struct->width, return_struct->height, "Hello World", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Window creation failed." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Enable 60+ fps: 0. */
    // glfwSwapInterval(0);
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);


    GLenum err = glewInit();
    if (GLEW_OK != err)
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;

    std::cerr << std::endl;

    ErrorInit(ERROR_LEVEL);
    PrintInfo();

    return window;
}

void call_world(World world) {
    return;
}

void TestBody() {
    Body ba("A");
    Body bb;

    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << ba;
    ofs.close();

    ifs.open("test.txt");
    ifs >> bb;
    std::cout << bb;
}

void TestWorld() {
    World w, w2;
    w.AddBody("A");
    w.AddBody("B");
    w.AddBody("C");


    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << w;
    ofs.close();

    ifs.open("test.txt");
    ifs >> w2;
    std::cout << "printing world " <<  w2.GetCount() << std::endl;
    std::cout << w2;
}

void TestCamera() {

    Camera camera;
    std::cout << camera;
    camera.SetCenter(glm::vec3(10.0f, 20.0f, 0.0f));
    std::cout << camera;

    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << camera;
    ofs.close();

    ifs.open("test.txt");
    ifs >> camera;
    std::cout << camera;
}

int main(void) {

    // std::cout.precision(32);

    //
    // Scene scene;
    // World w;

    // std::ifstream ifs;
    // ifs.open("../scenes/sun_earth/world.cfg");
    // ifs >> w;
    // std::cout << w;
    // TestWorld();
    // TestBody();
    // exit(0);

    GLFWwindow* window = init();

    if (!window) return -1;

    // const int pi = 18;
    // const int pc = 24;
    // const int ii = 12;
   // //
    // float positions[pi] = {-1.0, -0.8,
    //                       -0.7, -0.2,
    //                       -0.2, -0.4,
    //
    //                       0.8, 0.6,
    //
    //                       0.0, 0.5,
    //                       -0.5, 1.0};

   //  float positions[pi] = {-1, 0,
   //                     -0.8, 0,
   //                     -0.2, 0,
   //
   //                     0.0, 0,
   //                     0.1, 0,
   //                     0.4, 0,
   //
   //                      0.5, 0,
   //                      0.6, 0,
   //                      0.7, 0,};
   // //
   // unsigned char colors[pc] = {255, 255, 255, 255,
   //                        255, 255, 255, 255,
   //                        255, 255, 255, 255,
   //
   //
   //                        255, 0, 0, 255,
   //                        255, 0, 0, 255,
   //                        255, 0, 0, 255};
   //  unsigned int indices[ii] = {0, 1, 2, 0xFFFFFFFF, 3, 4, 5};
    // unsigned int indices[ii] = {0, 1, 2, 0xFFFFFFFF, 3, 4, 5, 0xFFFFFFFF, 6, 7, 8, 0xFFFFFFFF};

    // VertexArray va;
    // VertexBuffer vb(positions, sizeof (float) * pi);
    // vb.UnBind();
    // VertexBuffer vb2(colors, sizeof (unsigned char) * pc);
   //  // vb2.UnBind();
    // IndexBuffer ib(indices, ii);
   //  // ib.UnBind();
    // VertexBufferLayout layout, layout2;
    // layout.Push<float>(2);
   //  layout2.Push<unsigned char>(4);
    // va.AddBuffer(vb, 0, layout);
   //  // vb.UnBind();
   //  va.AddBuffer(vb2, 1, layout2);
   //  // vb2.UnBind();
    // ShaderSources sources = Shader::GetShaderSources("lines.vert",  "lines.frag");
    // Shader shader(sources);
    // shader.Bind();
    // Renderer renderer;
    // glEnable(GL_PRIMITIVE_RESTART);
    // glPrimitiveRestartIndex(0xFFFFFFFF);


    // unsigned int va, vb1, vb2, ib;
    // glGenVertexArrays(1, &va);
    // glBindVertexArray(va);

    // glGenBuffers(1, &vb1);
    // glGenBuffers(1, &vb2);
    // glGenBuffers(1, &ib);
    //
    //
    // glBindBuffer(GL_ARRAY_BUFFER, vb1);
    // glBufferData(GL_ARRAY_BUFFER, sizeof (float) * pi, positions, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    // glBindBuffer(GL_ARRAY_BUFFER, vb2);
    // glBufferData(GL_ARRAY_BUFFER, sizeof (unsigned char) * pc, colors, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, ii * sizeof (unsigned int), indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //
    // glBindBuffer(GL_ARRAY_BUFFER, vb1);
    // glVertexAttribPointer(0, 2,  GL_FLOAT, false, 0, 0);
    // glEnableVertexAttribArray(0);
    //
    // glBindBuffer(GL_ARRAY_BUFFER, vb2);
    // glVertexAttribPointer(1, 4,  GL_UNSIGNED_BYTE, false, 0, 0);
    // glEnableVertexAttribArray(1);
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    //
    // ShaderSources sources = Shader::GetShaderSources("lines.vert", "lines.frag");
    // Shader shader(sources);
    //
    // shader.Bind();

    // std::string scene_name = "scene";
    // std::filesystem::create_directories("../scenes/" + scene_name);
    // exit(0);

    // TestCamera();


    // for (int i = 0; i < 5; i++)
    //     std::cout << w;


    // glm::vec3 p1(1.0f, 2.0f, 0.0f);
    // std::cout << &(p1.x) << " " << &(p1.y) << std::endl;
    // exit(0);



    Gui gui(window);
    Simulation simulation(window, gui);

    glfwSetWindowUserPointer(window, (void *) &simulation);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);

    float xmid = 1920 / 2;
    float ymid = 1080 / 2;


    glm::vec3 center(xmid, ymid, 0.0f);
    glm::vec3 earth_pos(-DISTANCE_MOON_EARTH / 2, 0, 0.0f);
    glm::vec3 moon_pos(DISTANCE_MOON_EARTH/2, 0, 0.0f);
    glm::vec3 moon_v(0.0f, VELOCITY_MOON, 0.0f);

    simulation.WorldAddBody("earth", earth_pos, glm::vec3(0.0f), RADIUS_EARTH, MASS_EARTH);
    simulation.WorldAddBody("moon", moon_pos, moon_v, RADIUS_MOON, MASS_MOON, {0, 0, 255, 255});
    // // simulation.WorldAddBody("earth", center, glm::vec3(0.01f), 1.0f, MASS_EARTH);

    simulation.CameraSetCenter(earth_pos);
    simulation.CameraFit();


    int i = 0;

    while (!glfwWindowShouldClose(window)) {
        // sleep(0.6);
        simulation.Render();
        // std::cout << "Is enabled: " << glIsEnabled(GL_PRIMITIVE_RESTART) << std::endl;

        i++;
        if (simulation.Running()) {
            simulation.Step();
        }

        // renderer.DrawLineStrip(va, ib, shader);
        // glDrawElements(GL_LINE_STRIP, ii, GL_UNSIGNED_INT, nullptr);


        if (i % 60 == 0) {
            // simulation.CameraInfo();
        }
        // std::cout.precision(20);
        // total = world.TotalEnergy();
        // kinetic = world.KineticEnergy();
        // potential = world.PotentialEnergy();
        // if (t != total) {
        //     std::cout << "Total: " << total << " | ";
        // }
        // if (k != kinetic) {
        //     std::cout << "Kinetic: " << kinetic << " | ";
        // }
        // if (p != potential) {
        //     std::cout << "Potential:" << potential;
        // }
        // if (p != potential || k != kinetic || t != total) {
        //     t = total;
        //     k = kinetic;
        //     p = potential;
        //     std::cout << std::endl << i << std::endl;
        //     i = 0;
        // }

        // std::cout << world.TotalEnergy() << " " << world.PotentialEnergy() << " " << world.KineticEnergy() << std::endl;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
