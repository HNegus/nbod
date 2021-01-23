// #define GLM_PRECISION_HIGHP_DOUBLE

#include "utils.hpp"

#include "errors.hpp"

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

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);


    int i = 0;

    while (!glfwWindowShouldClose(window)) {

        simulation.Render();

        i++;
        if (simulation.Running()) {
            simulation.Step();
        }
        // TODO remove
        // simulation.CameraInfo();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
