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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "body.hpp"
#include "world.hpp"

E_ErrorLevels ERROR_LEVEL = HIGH;
int SCREEN_WIDTH, SCREEN_HEIGHT;
float ZOOM_LEVEL = -4.0f * pow(10, 8);
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

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* return_struct = glfwGetVideoMode(monitor);
    SCREEN_WIDTH = return_struct->width;
    SCREEN_HEIGHT = return_struct->height;
    ZOOM_RATIO = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
    ZOOMX = ZOOM_RATIO * ZOOM_LEVEL;
    ZOOMY = ZOOM_LEVEL;
    /* Hint correct OpenGL version to GLFW. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);


    /* Create a windowed mode window and its OpenGL context */
    // window = glfwCreateWindow(1920, 1080, "Hello World", glfwGetPrimaryMonitor(), nullptr);
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", nullptr, nullptr);

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


    float scale = -ZOOM_LEVEL;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        MOVE.x += scale * 1;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        MOVE.x -= scale * 1;
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        MOVE.y -= scale * 1;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        MOVE.y += scale * 1;
    } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        play_pause();
    } else if (key == GLFW_KEY_G
         && action == GLFW_PRESS) {
        toggle_gui();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        MOVE.x += scale * 5;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        MOVE.x -= scale * 5;
    } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        MOVE.y -= scale * 5;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        MOVE.y += scale * 5;
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << yoffset << std::endl;

    float scale = 1.0f;

    if (ZOOM_LEVEL < 100) {
        scale = 10.0f;
    } else if (ZOOM_LEVEL < 200) {
        scale = 10.0f;
    } else if (ZOOM_LEVEL < 300) {
        scale = 10.0f;
    } else if (ZOOM_LEVEL < 400) {
        scale = 3.0f;
    }
    scale = DISTANCE_MOON_EARTH / 1000.0f;
    // float scale = 5.0f;
    ZOOM_LEVEL += scale * yoffset;

    if (ZOOM_LEVEL >= SCREEN_HEIGHT - ZOOMY - 1) {
        ZOOM_LEVEL -= scale * yoffset;
    } else {
        ZOOMX = ZOOM_RATIO * ZOOM_LEVEL;
        ZOOMY = ZOOM_LEVEL;
    }

    std::cout << "Zoom level: " << ZOOM_LEVEL << std::endl;
    std::cout << "scale: " << scale << std::endl;
    std::cout << "Zoom ratio:" << ZOOM_RATIO << std::endl;
    std::cout << "X:" << ZOOMX << std::endl;
    std::cout << "Y:" << ZOOMY << std::endl;
    std::cout << SCREEN_HEIGHT - ZOOMY << std::endl;

    std::cout << std::endl;

    return;
}

int main(void) {

    GLFWwindow* window = init();

    if (!window) return -1;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


    // float positions[32] = {20.0f,    20.0f,       0.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.8f,
    //                        40.0f,  20.0f,       1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.8f,
    //                        40.0f,  40.0f,     1.0f, 1.0f,     1.0f, 0.0f, 1.0f, 0.8f,
    //                        20.0f,    40.0f,     0.0f, 1.0f,     0.0f, 1.0f, 0.0f, 0.8f
    // };




    float xmid = SCREEN_WIDTH / 2;
    float ymid = SCREEN_HEIGHT / 2;
    float radius = 1.0f;

    World world;
    world.AddBody("earth", -DISTANCE_MOON_EARTH / 2, 0, RADIUS_EARTH, MASS_EARTH, 0, 0);
    world.AddBody("moon", DISTANCE_MOON_EARTH / 2, 0, RADIUS_MOON, MASS_MOON, 0, VELOCITY_MOON);
    world.AddBody("earth2", DISTANCE_MOON_EARTH / 2, DISTANCE_MOON_EARTH / 2, RADIUS_EARTH, MASS_EARTH, 0, 0);
    world.AddBody("earth3", DISTANCE_MOON_EARTH / 2, -DISTANCE_MOON_EARTH / 1.3, RADIUS_EARTH, MASS_EARTH * 0.8, 0, 0);

    // world.AddBody(xmid + 20, ymid + 20, radius + 2, radius);

    world.Bodies();


    std::cout << SCREEN_HEIGHT << std::endl;

    float positions[40] = {xmid - radius, ymid - radius, radius, xmid, ymid,
                          xmid + radius, ymid - radius, radius, xmid, ymid,
                          xmid - radius, ymid + radius, radius, xmid, ymid,
                          xmid + radius, ymid + radius, radius, xmid, ymid,

                          xmid - radius + 10, ymid - radius + 10, radius, xmid + 10, ymid + 10,
                          xmid + radius + 10, ymid - radius + 10, radius, xmid + 10, ymid + 10,
                          xmid - radius + 10, ymid + radius + 10, radius, xmid + 10, ymid + 10,
                          xmid + radius + 10, ymid + radius + 10, radius, xmid + 10, ymid + 10
                          };



    unsigned int indices[12] = {0, 1, 2,
                               1, 3, 2,
                               4, 5, 6,
                               5, 7, 6};


  // for (int i = 0; i < 40; i++) {
  //     std::cout << positions[i] << " ";
  // }
  // world.Renew();

  std::cout << std::endl;
   // VertexArray va;
   VertexArray va;

   // VertexBuffer vb(positions, 2 * 5 * 4 * sizeof (float));
   VertexBuffer vb(world.vbdata(), world.vbsize());
   // VertexBuffer vb(positions, world.vbsize());

   // VertexBuffer vb2(world.Data().data(), 40);
   // IndexBuffer ib(indices, 2 * 6);
   IndexBuffer ib(world.ibdata(), world.ibsize());
   // IndexBuffer ib(indices, world.ibsize());

   // VertexBufferLayout layout;
   VertexBufferLayout layout;
   layout.Push<float>(2);
   layout.Push<float>(1);
   layout.Push<float>(2);
   va.AddBuffer(vb, layout);


    ShaderSources sources = Shader::GetShaderSources("vertex.glsl",
                                                     "fragment.glsl");

    Shader shader(sources);
    shader.Bind();
    // shader.SetUniform4f("u_Color", 0.9, 0.0, 0.0, 0.5);
    glm::vec3 translate(0, 0, 0);
    glm::mat4 proj = glm::ortho(0.0f, (float) SCREEN_WIDTH, 0.0f, (float) SCREEN_HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), translate);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);

    Gui gui(window);
    Renderer renderer;

    va.Bind();
    ib.Bind();

    int i = 0;
    while (!glfwWindowShouldClose(window)) {
        // if (i % 80 == 0) {
        //     world.Do();
        //     vb.Renew(world.vbdata(), world.vbsize());
        //     ib.Renew(world.ibdata(), world.ibsize());
        //     va.Renew(vb, layout);
        //     va.Bind();
        //     ib.Bind();
        //
        // }
        i++;
        std::cout << play << std::endl;
        if (play) {
            world.Step();
            vb.Renew(world.vbdata(), world.vbsize());
            ib.Renew(world.ibdata(), world.ibsize());
            va.Renew(vb, layout);
        }
        // world.Bodies();

        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);
        proj = glm::ortho(0.0f + ZOOMX, (float) SCREEN_WIDTH - ZOOMX, 0.0f + ZOOMY, (float) SCREEN_HEIGHT - ZOOMY, -1.0f, 1.0f);
        view = glm::translate(glm::mat4(1.0f), translate + MOVE);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Clear();

        renderer.Draw(va, ib, shader);
        if (show_gui) {
            gui.NewFrame();
            ImGui::ShowDemoWindow();
            ImGui::Checkbox("Play", &play);

            gui.Render();
        }
        // return;


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
