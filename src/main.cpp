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


E_ErrorLevels ERROR_LEVEL = HIGH;


static GLFWwindow* init() {

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

    GLFWwindow* window = init();

    if (!window) return -1;

    float positions[16] = {-1.0, -0.8, -0.7, -0.2, -0.7, -0.2, -0.2, -0.4, -0.2, -0.4, 0.8, 0.6};

    VertexArray va;
    VertexBuffer vb(positions, sizeof(float) * 16);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    ShaderSources sources = Shader::GetShaderSources("test.vert", "test.frag");
    Shader shader(sources);
    // va.Bind();
    // shader.Bind();
    Renderer renderer;
    // std::string scene_name = "scene";
    // std::filesystem::create_directories("../scenes/" + scene_name);
    // exit(0);

    // TestCamera();

    std::cout << "!!!!!!!!!!1got here" << std::endl;

    // for (int i = 0; i < 5; i++)
    //     std::cout << w;






    Gui gui(window);
    Simulation simulation(window, gui);

    // glfwSetWindowUserPointer(window, (void *) &simulation);
    // glfwSetScrollCallback(window, scroll_callback);
    // glfwSetKeyCallback(window, key_callback);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


    // float xmid = 1920 / 2;
    // float ymid = 1080 / 2;
    // float radius = 1.0f;
    //
    //
    //
    // glm::vec3 earth_pos(-DISTANCE_MOON_EARTH / 2, 0.0f, 0.0f);
    // glm::vec3 moon_pos(DISTANCE_MOON_EARTH / 2, 0.0f, 0.0f);
    // glm::vec3 moon_v(0.0f, VELOCITY_MOON, 0.0f);
    //
    // simulation.WorldAddBody("earth", earth_pos, glm::vec3(0.0f), RADIUS_EARTH, MASS_EARTH);
    // simulation.WorldAddBody("moon", moon_pos, moon_v, RADIUS_MOON, MASS_MOON);
    // for (int i = 0; i < 100; i++)
    //     simulation.WorldAddBody("earth", earth_pos, glm::vec3(0.0f), RADIUS_EARTH, MASS_EARTH);
    //
    // std::cout << simulation.WorldBodies()[0];
    // simulation.CameraSetCenter(earth_pos);
    // simulation.Init();



    int i = 0;


    while (!glfwWindowShouldClose(window)) {

        // simulation.Render();
        //
        // i++;
        // if (simulation.Running()) {
        //     simulation.Step();
        // }

        renderer.DrawLines(va, shader);



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
