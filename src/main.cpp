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
#include "camera.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "body.hpp"
#include "world.hpp"


E_ErrorLevels ERROR_LEVEL = HIGH;
int SCREEN_WIDTH, SCREEN_HEIGHT;
float ZOOM_LEVEL = -4.0f * pow(10, 8);
// float ZOOM_LEVEL = 0.0f;
float ZOOM_RATIO, ZOOMX, ZOOMY;
glm::vec3 MOVE(0, 0, 0);

static bool play = 0;
static bool show_gui = 1;



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

void play_pause() {
    play = !play;
}

void toggle_gui () {
    show_gui = !show_gui;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {


    float scalex = 0.01 * ZOOMX;
    float scaley = 0.01 * ZOOMY;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        MOVE.x -= scalex * 1;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        MOVE.x += scalex * 1;
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        MOVE.y += scaley * 1;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        MOVE.y -= scaley * 1;
    } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        play_pause();
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        toggle_gui();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        MOVE.x -= scalex * 5;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        MOVE.x += scalex * 5;
    } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        MOVE.y += scaley * 5;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        MOVE.y -= scaley * 5;
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // std::cout << yoffset << std::endl;

    // float scale = 1.0f;
    //
    // if (ZOOM_LEVEL < 100) {
    //     scale = 10.0f;
    // } else if (ZOOM_LEVEL < 200) {
    //     scale = 10.0f;
    // } else if (ZOOM_LEVEL < 300) {
    //     scale = 10.0f;
    // } else if (ZOOM_LEVEL < 400) {
    //     scale = 3.0f;
    // }
    // scale = ZOOMX * 0.1;
    // float scale = 5.0f;
    // ZOOM_LEVEL += scale * yoffset;

    // if (ZOOM_LEVEL >= SCREEN_HEIGHT - ZOOMY - 1) {
        // ZOOM_LEVEL -= scale * yoffset;
    // } else {
        // ZOOMX = ZOOM_RATIO * ZOOM_LEVEL;
        // ZOOMY = ZOOM_LEVEL;
    // }

    // std::cout << "Zoom level: " << ZOOM_LEVEL << std::endl;
    // std::cout << "scale: " << scale << std::endl;
    // std::cout << "Zoom ratio:" << ZOOM_RATIO << std::endl;
    // std::cout << "X:" << ZOOMX << std::endl;
    // std::cout << "Y:" << ZOOMY << std::endl;
    // std::cout << SCREEN_HEIGHT - ZOOMY << std::endl;
    //
    // std::cout << std::endl;

    // void *ptr = NULL;
    Camera *cam = (Camera *) glfwGetWindowUserPointer(window);
    cam->Zoom(yoffset);

    return;
}

int main(void) {

    GLFWwindow* window = init();

    if (!window) return -1;



    Camera *camera = new Camera;
    std::cout << camera << std::endl;

    // camera.Do();
    glfwSetWindowUserPointer(window, (void *) camera);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


    float xmid = camera->ScreenWidth() / 2;
    float ymid = camera->ScreenHeight() / 2;
    float radius = 1.0f;

    World world;
    // world.AddBody("earth", -20, ymid, 100, MASS_EARTH, 0, 0);
    // world.AddBody("moon", 200, ymid, 10, MASS_MOON, 0, VELOCITY_MOON);

    world.AddBody("earth", -DISTANCE_MOON_EARTH / 2, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    world.AddBody("moon", DISTANCE_MOON_EARTH / 2, 0, RADIUS_MOON, MASS_MOON, 0, VELOCITY_MOON);
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
    shader.SetUniformMat4f("u_MVP", camera->MVP());

    Gui gui(window);
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
        if (play) {
            world.Step();
            vb.Renew(world.vbdata(), world.vbsize());
            ib.Renew(world.ibdata(), world.ibsize());
            va.Renew(vb, layout);
        }
        // world.Bodies();

        glfwSetScrollCallback(window, scroll_callback);
        // glfwSetKeyCallback(window, key_callback);
        // proj = glm::ortho(0.0f + ZOOMX, (float) SCREEN_WIDTH - ZOOMX, 0.0f + ZOOMY, (float) SCREEN_HEIGHT - ZOOMY, -1.0f, 1.0f);
        // view = glm::translate(glm::mat4(1.0f), translate + MOVE);
        // model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        // mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", camera->MVP());
        renderer.Clear();

        renderer.Draw(va, ib, shader);
        if (show_gui) {
            gui.NewFrame();
            ImGui::ShowDemoWindow();
            ImGui::Checkbox("Play", &play);

            gui.Render();
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
