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





int main(void) {

    GLFWwindow* window = init();

    if (!window) return -1;

    Gui gui(window);
    Simulation simulation(window, gui);

    glfwSetWindowUserPointer(window, (void *) &simulation);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


    float xmid = 1920 / 2;
    float ymid = 1080 / 2;
    float radius = 1.0f;



    glm::vec3 earth_pos(-DISTANCE_MOON_EARTH / 2, 0.0f, 0.0f);
    glm::vec3 moon_pos(DISTANCE_MOON_EARTH / 2, 0.0f, 0.0f);
    glm::vec3 moon_v(0.0f, VELOCITY_MOON, 0.0f);

    simulation.WorldAddBody("earth", earth_pos, MASS_EARTH, RADIUS_EARTH, glm::vec3(0.0f));
    simulation.WorldAddBody("moon", moon_pos, MASS_MOON, RADIUS_MOON, moon_v);
    // simulation.WorldAddBody("moon2", moon_pos, MASS_MOON, RADIUS_MOON, moon_v);
    // simulation.WorldAddBody("earth", glm::vec3(1000.0f, ymid, 1.0f), 10.0f, 100.0f, glm::vec3(0.0f));
    // simulation.WorldAddBody("earth2", glm::vec3(1000.0f, ymid * 3, 1.0f), 10.0f, 100.0f, glm::vec3(0.0f));
    // simulation.WorldAddBody("earth2", glm::vec3(-4000.0f, ymid * 3, 1.0f), 10.0f, 100.0f, glm::vec3(0.0f));


    // simulation.WorldAddBody("moon", glm::vec3(0.0f) + 100.0f, 1.0f, 50.0f, moon_v);

    simulation.CameraSetCenter(earth_pos);
    simulation.Init();

    // world.AddBody("earth", -DISTANCE_MOON_EARTH / 2, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    // world.AddBody("moon", DISTANCE_MOON_EARTH / 2, 0, RADIUS_MOON, MASS_MOON, 0, VELOCITY_MOON);
    // world.AddBody("moon2", -DISTANCE_MOON_EARTH, 0, RADIUS_MOON, MASS_MOON, 0, -VELOCITY_MOON);

    // world.AddBody("earth2", DISTANCE_MOON_EARTH, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    // world.AddBody("earth3", DISTANCE_MOON_EARTH / 2, -DISTANCE_MOON_EARTH / 1.3, RADIUS_EARTH, MASS_EARTH * 0.8, 0, 0);
    // simulation.Do();
    // exit(0);






    int i = 0;


    while (!glfwWindowShouldClose(window)) {

        simulation.Render();

        i++;
        if (simulation.Running()) {
            simulation.Step();
        }



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
