#pragma once

#include "utils.hpp"
#include "simulation.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    Simulation *simulation = (Simulation *) glfwGetWindowUserPointer(window);
    glm::vec3 move(0.0f, 0.0f, 0.0f);

    if (!ImGui::GetIO().WantCaptureKeyboard) {

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            move.x += 1;
        } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            move.x -= 1;
        } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            move.y -= 1;
        } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            move.y += 1;
        } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            simulation->TogglePlay();
        } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
            simulation->GuiToggle();
        } else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            simulation->CameraCenter();
        } else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
            simulation->TrackToggle();
        } else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
            simulation->HistoryToggle();
        } else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
            simulation->VariableTimeStepToggle();
        } else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
            simulation->LimitToggle();
        }


        if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
            move.x +=  5;
        } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
            move.x -= 5;
        } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
            move.y -= 5;
        } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
            move.y += 5;
        }

        simulation->CameraMove(move);
    }


    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    if (!ImGui::GetIO().WantCaptureMouse) {
        Simulation *simulation = (Simulation *) glfwGetWindowUserPointer(window);
        simulation->CameraZoom(yoffset);
    }
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    return;
}
