#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils.hpp"

class Gui {

private:
    GLFWwindow *m_window;
    ImGuiContext* m_context;


public:
    Gui(GLFWwindow *window);
    ~Gui();


    void NewFrame() const;
    void Render() const;

};
