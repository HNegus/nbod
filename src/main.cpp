#include "utils.hpp"

#include "errors.hpp"

#include "shader.hpp"
#include "vertexbufferlayout.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "vertexarray.hpp"
#include "texture.hpp"

#include "renderer.hpp"
#include "gui.hpp"
#include "simulation.hpp"
#include "input.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    // std::cout << camera << std::endl;

    // camera.Do();
    glfwSetWindowUserPointer(window, (void *) &simulation);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


    float xmid = 1920 / 2;
    float ymid = 1080 / 2;
    float radius = 1.0f;

    World world;
    // world.AddBody("earth", -20, ymid, 100, MASS_EARTH, 0, 0);
    // world.AddBody("moon", 200, ymid, 10, MASS_MOON, 0, VELOCITY_MOON);

    world.AddBody("earth", -DISTANCE_MOON_EARTH / 2, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    world.AddBody("moon", DISTANCE_MOON_EARTH / 2, 0, RADIUS_MOON, MASS_MOON, 0, VELOCITY_MOON);
    simulation.CameraSetCenter(glm::vec3(-DISTANCE_MOON_EARTH / 2, 0.0f, 0.0f));
    // world.AddBody("moon2", -DISTANCE_MOON_EARTH, 0, RADIUS_MOON, MASS_MOON, 0, -VELOCITY_MOON);

    // world.AddBody("earth2", DISTANCE_MOON_EARTH, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    // world.AddBody("earth3", DISTANCE_MOON_EARTH / 2, -DISTANCE_MOON_EARTH / 1.3, RADIUS_EARTH, MASS_EARTH * 0.8, 0, 0);


   VertexArray va;

   VertexBuffer vb(world.vbdata(), world.vbsize());
   IndexBuffer ib(world.ibdata(), world.ibsize());

   VertexBufferLayout layout;
   layout.Push<float>(2);
   layout.Push<float>(1);
   layout.Push<float>(2);
   va.AddBuffer(vb, layout);


    ShaderSources sources = Shader::GetShaderSources("vertex.glsl",
                                                     "fragment.glsl");

    Shader shader(sources);
    shader.Bind();
    shader.SetUniformMat4f("u_MVP", simulation.MVP());

    Renderer renderer;

    va.Bind();
    ib.Bind();

    int i = 0;
    double t = world.TotalEnergy();
    double k = world.KineticEnergy();
    double p = world.PotentialEnergy();
    double total, kinetic, potential;

    while (!glfwWindowShouldClose(window)) {

        i++;
        if (simulation.Running()) {
            world.Step();
            vb.Renew(world.vbdata(), world.vbsize());
            ib.Renew(world.ibdata(), world.ibsize());
            va.Renew(vb, layout);
        }
        // world.Bodies();

        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", simulation.MVP());
        renderer.Clear();

        renderer.Draw(va, ib, shader);
        simulation.GuiRender();

        if (i % 60 == 0) {
            // camera.Info();
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
